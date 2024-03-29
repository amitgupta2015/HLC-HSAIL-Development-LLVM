//===- HSAILRegisterInfo.cpp - HSAIL Register Information -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the HSAIL implementation of the TargetRegisterInfo class.
// This file is responsible for the frame pointer elimination optimization
// on HSAIL.
//
//===----------------------------------------------------------------------===//

#include "HSAIL.h"
#include "HSAILRegisterInfo.h"
#include "HSAILMachineFunctionInfo.h"
#include "HSAILSubtarget.h"
#include "HSAILTargetMachine.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
//#include "llvm/CodeGen/MachineLocation.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
using namespace llvm;

static cl::opt<int> HSAILReg32PressureLimit(
  "hsail-reg32-pressure-limit", cl::Hidden, cl::init(24),
  cl::desc("Register pressure limit for 32 bit HSAIL registers"));

static cl::opt<int> HSAILReg64PressureLimit(
  "hsail-reg64-pressure-limit", cl::Hidden, cl::init(18),
  cl::desc("Register pressure limit for 64 bit HSAIL registers"));

static cl::opt<int> HSAILRegSlots(
  "hsail-reg-slots", cl::Hidden, cl::init(0),
  cl::desc("A number of 64-bit slots allocated for $s registers"));

HSAILRegisterInfo::HSAILRegisterInfo(HSAILSubtarget &st)
  : HSAILGenRegisterInfo(0, 0), ST(st) {}

/// getCalleeSavedRegs - Return a null-terminated list of all of the
/// callee saved registers on this target. The register should be in the
/// order of desired callee-save stack frame offset. The first register is
/// closed to the incoming stack pointer if stack grows down, and vice versa.
const uint16_t*
HSAILRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const
{
  static const uint16_t CalleeSavedRegs[] = { 0 };
  return CalleeSavedRegs;
}

/// getRegsAvailable - Return all available registers in the register class
/// in Mask.
BitVector
HSAILRegisterInfo::getRegsAvailable(const TargetRegisterClass *RC) const
{
  BitVector Mask(getNumRegs());
  for (TargetRegisterClass::iterator I = RC->begin(), E = RC->end();
       I != E; ++I)
    Mask.set(*I);
  return Mask;
}

/// getReservedRegs - Returns a bitset indexed by physical register number
/// indicating if a register is a special register that has particular uses
/// and should be considered unavailable at all times, e.g. SP, RA. This is
/// used by register scavenger to determine what registers are free.
BitVector
HSAILRegisterInfo::getReservedRegs(const MachineFunction &MF) const
{
  BitVector Reserved(getNumRegs());

  // We can have up to 128 s-registers, but we should have (s + 2*d + 4*q) <= 128.
  // Let's calulate the number of 32 and 64 bit VRs used in the function
  // and partition register file accordingly.
  static std::map<unsigned, unsigned> FunctionToRegPartionMap;
  unsigned NumSlotsTotal = HSAIL::GPR64RegClass.getNumRegs();
  // Default register file partitioning 64 s-regs + 32 d-regs, RegSlots = 32.
  unsigned RegSlots = NumSlotsTotal / 2;

  // First query for this function, calculate register use
  if (FunctionToRegPartionMap.find(MF.getFunctionNumber()) ==
      FunctionToRegPartionMap.end()) {
    const MachineRegisterInfo& RI = MF.getRegInfo();
    unsigned rc32 = 0, rc64 = 0;
    for( unsigned i = 0, e = RI.getNumVirtRegs(); i != e; ++i) {
      switch ( RI.getRegClass(index2VirtReg(i))->getSize() ) {
      case 4:
        rc32++;
        break;
      case 8:
        rc64++;
        break;
      }
    }

    if (HSAILRegSlots > 0) {
      RegSlots = HSAILRegSlots;
    } else {
      // Calculate register file partitioning. We have 64 allocatable slots which
      // are either 1 d-register or a pair of s-registers. 8 slots are reserved
      // for 16 s-registers $s0..$s15, 8 are for 8 d-registers $d0..$d7.
      // Default partitioning is 64 s-registers + 32 d-registers, which is
      // RegSlots = 32

      // If we have a small amount of 64 bit VRs, but high 32 bit register
      // pressure reallocate slots to decrease 64 bit registers
      if( rc64 < (NumSlotsTotal - RegSlots) && rc32 > (RegSlots*2) ) {
        RegSlots = NumSlotsTotal - rc64;
      }
      // The opposite situation, we have a small demand on 32 bit registers but
      // high pressure for 64 bit
      else if( rc32 < (RegSlots*2) && rc64 > (NumSlotsTotal - RegSlots) ) {
        RegSlots = (rc32+1) / 2;
      }
    }

    // Always preserve room for at least 16 s-registers and 8 d-registers
    if (RegSlots < 8) RegSlots = 8;
    else if (RegSlots > (NumSlotsTotal - 8)) RegSlots = NumSlotsTotal - 8;

    FunctionToRegPartionMap[MF.getFunctionNumber()] = RegSlots;
  } else {
    RegSlots = FunctionToRegPartionMap[MF.getFunctionNumber()];
  }

  unsigned Reg;
  unsigned LastSReg = HSAIL::S0 + HSAIL::GPR32RegClass.getNumRegs() - 1;
  for( Reg = HSAIL::S0 + RegSlots*2 ; Reg <= LastSReg; ++Reg ) {
    Reserved.set(Reg);
  }
  unsigned LastDReg = HSAIL::D0 + HSAIL::GPR64RegClass.getNumRegs() - 1;
  for( Reg = HSAIL::D0 + (NumSlotsTotal - RegSlots) ; Reg <= LastDReg; ++Reg ) {
    Reserved.set(Reg);
  }

  return Reserved;
}

bool
HSAILRegisterInfo::trackLivenessAfterRegAlloc(const MachineFunction &MF) const {
  // Only enable when post-RA scheduling is enabled and this is needed.
  // TODO: HSA
  return true;
}


/// getPointerRegClass - Returns a TargetRegisterClass used for pointer
/// values.  If a target supports multiple different pointer register classes,
/// kind specifies which one is indicated.
const TargetRegisterClass*
HSAILRegisterInfo::getPointerRegClass(const MachineFunction &MF,
		                      unsigned Kind) const
{
  assert(Kind == 0);

  if (ST.is64Bit())
    return &HSAIL::GPR64RegClass;
  return &HSAIL::GPR32RegClass;
}

/// requiresRegisterScavenging - returns true if the target requires (and can
/// make use of) the register scavenger.
bool
HSAILRegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const
{
  return true;
}

/// requiresFrameIndexScavenging - returns true if the target requires post
/// PEI scavenging of registers for materializing frame index constants.
bool
HSAILRegisterInfo::requiresFrameIndexScavenging(const MachineFunction &MF) const
{
  return true;
}

/// eliminateCallFramePseudoInstr - This method is called during prolog/epilog
/// code insertion to eliminate call frame setup and destroy pseudo
/// instructions (but only if the Target is using them).  It is responsible
/// for eliminating these instructions, replacing them with concrete
/// instructions.  This method need only be implemented if using call frame
/// setup/destroy pseudo instructions.
///
void
HSAILRegisterInfo::eliminateCallFramePseudoInstr(MachineFunction &MF,
                                                 MachineBasicBlock &MBB,
                                                 MachineBasicBlock::iterator MI) const
{
  MBB.erase(MI);
}


/// saveScavengerRegister - Spill the register so it can be used by the
/// register scavenger. Return true if the register was spilled, false
/// otherwise. If this function does not spill the register, the scavenger
/// will instead spill it to the emergency spill slot.
///
bool
HSAILRegisterInfo::saveScavengerRegister(MachineBasicBlock &MBB,
                                         MachineBasicBlock::iterator I,
                                         MachineBasicBlock::iterator &UseMI,
                                         const TargetRegisterClass *RC,
                                         unsigned Reg) const
{
  MachineFunction *MF = MBB.getParent();
  MachineFrameInfo *MFI = MF->getFrameInfo();

  return saveScavengerRegisterToFI(MBB, I, UseMI, RC, Reg,
    MFI->CreateSpillStackObject(RC->getSize(), RC->getAlignment()));
}

static int getScavengingFrameIndex(RegScavenger *RS) {

  SmallVector<int, 8> FI;
  RS->getScavengingFrameIndices(FI);
  return FI[0];

}

// Save scavenger register and provide a frame index for it.
bool
HSAILRegisterInfo::saveScavengerRegisterToFI(MachineBasicBlock &MBB,
                                             MachineBasicBlock::iterator I,
                                             MachineBasicBlock::iterator &UseMI,
                                             const TargetRegisterClass *RC,
                                             unsigned Reg,
                                             int FI) const
{
  const HSAILInstrInfo &HII =
      *static_cast<const HSAILInstrInfo*>(ST.getInstrInfo());
  RegScavenger *RS = HII.getRS();

  assert(RS != 0 && "Register scavenger has not been created");

  // Setup emergency spill location
  RS->addScavengingFrameIndex(FI);

  assert(getScavengingFrameIndex(RS) >= 0 &&
         "Cannot scavenge register without an emergency spill slot!");

  // Store the scavenged register to its stack spill location
  HII.storeRegToStackSlot(MBB, I, Reg, true /* isKill */, getScavengingFrameIndex(RS), RC, this);

  // Restore the scavenged register before its use (or first terminator).
  HII.loadRegFromStackSlot(MBB, UseMI, Reg, getScavengingFrameIndex(RS), RC, this);

  // HSAIL Target saves/restores the scavenged register
  return true;
}

/// eliminateFrameIndex - This method must be overriden to eliminate abstract
/// frame indices from instructions which may use them.  The instruction
/// referenced by the iterator contains an MO_FrameIndex operand which must be
/// eliminated by this method.  This method may modify or replace the
/// specified instruction, as long as it keeps the iterator pointing at the
/// finished product. SPAdj is the SP adjustment due to call frame setup
/// instruction.
void
HSAILRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                       int SPAdj,
                                       unsigned FIOperandNum,
                                       RegScavenger *RS) const
{
  assert(SPAdj == 0 && "Unexpected");
  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  // FrameIndex to Offset translation is usually performed by the
  // target-specific implementation of TargetFrameLowering
  const TargetFrameLowering *TFL = MF.getTarget().getSubtarget<HSAILSubtarget>().getFrameLowering();
  
  unsigned int y = MI.getNumOperands();
  for (unsigned int x = 0; x < y; ++x) {
    if (!MI.getOperand(x).isFI()) {
      continue;
    }
    int FrameIndex = MI.getOperand(x).getIndex();
    int64_t Offset = TFL->getFrameIndexOffset(MF, FrameIndex);
    //int64_t Size = MF.getFrameInfo()->getObjectSize(FrameIndex);
    // An optimization is to only use the offsets if the size
    // is larger than 4, which means we are storing an array
    // instead of just a pointer. If we are size 4 then we can
    // just do register copies since we don't need to worry about
    // indexing dynamically
    if (MI.getOperand(x).isImm() == false)  {
      MI.getOperand(x).ChangeToImmediate(Offset);
    } else {
      MI.getOperand(x).setImm(Offset);
    }
  }
}

//===--------------------------------------------------------------------===//
/// Debug information queries.

/// getDwarfRegNum - Map a target register to an equivalent dwarf register
/// number.  Returns -1 if there is no equivalent value.  The second
/// parameter allows targets to use different numberings for EH info and
/// debugging info.
/*
int
HSAILRegisterInfo::getDwarfRegNum(unsigned RegNum, bool isEH) const
{
  // We only support 32 bit dwarf registers right now.
  unsigned Flavour = DWARFFlavour::HSAIL_Generic;
  return HSAILGenRegisterInfo::getDwarfRegNumFull( RegNum, Flavour );
}
*/
/// getFrameRegister - This method should return the register used as a base
/// for values allocated in the current stack frame.
unsigned
HSAILRegisterInfo::getFrameRegister(const MachineFunction &MF) const
{
  //This value is unused in LLVM
  return HSAIL::NoRegister;
}

/// getRARegister - This method should return the register where the return
/// address can be found.
unsigned
HSAILRegisterInfo::getRARegister() const
{
  assert(!"When do we hit this?");
  return 0;
}

/// getRegPressureLimit - Return the register pressure "high water mark" for
/// the specific register class. The scheduler is in high register pressure
/// mode (for the specific register class) if it goes over the limit.
unsigned
HSAILRegisterInfo::getRegPressureLimit(const TargetRegisterClass *RC,
                                         MachineFunction &MF) const
{
#ifdef _DEBUG
  std::string env("AMD_DEBUG_HSAIL_REGPRESSURELIMIT_");
  env.append(RC->getName());
  const char* pLimit = std::getenv(env.c_str());
  static unsigned reported = 0;
  reported++;
  if (pLimit) {
    unsigned limit = (unsigned)std::atoi(pLimit);
    if (reported <= getNumRegClasses())
      printf("Register pressure limit override for %s is %u\n", RC->getName(), limit);
    return limit;
  }
#endif
  if (RC == &HSAIL::GPR32RegClass) {
    return HSAILReg32PressureLimit;
  }
  if (RC == &HSAIL::GPR64RegClass) {
    return HSAILReg64PressureLimit;
  }
  return 0;
}

#define GET_REGINFO_MC_DESC
#define GET_REGINFO_TARGET_DESC
#include "HSAILGenRegisterInfo.inc"
