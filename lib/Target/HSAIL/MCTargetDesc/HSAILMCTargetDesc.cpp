//===-- HSAILMCTargetDesc.cpp - HSAIL Target Descriptions ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// \brief This file provides HSAIL specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "HSAILMCTargetDesc.h"
#include "HSAILMCAsmInfo.h"
#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

#include "BRIGDwarfStreamer.h"
#include "RawVectorOstream.h"

using namespace llvm;

// MC related code probably should be in MCTargetDesc subdir
static MCCodeGenInfo *createHSAILMCCodeGenInfo(StringRef TT, Reloc::Model RM,
                                               CodeModel::Model CM,
                                               CodeGenOpt::Level OL) {
  MCCodeGenInfo *X = new MCCodeGenInfo();
  X->InitMCCodeGenInfo(RM, CM, OL);
  return X;
}

static MCStreamer *createBRIGStreamer(const Target &T, StringRef TT,
                                      MCContext &Ctx, MCAsmBackend &TAB,
                                      raw_ostream &_OS, MCCodeEmitter *_Emitter,
                                      const MCSubtargetInfo &MSI, bool RelaxAll,
                                      bool NoExecStack) {
  Triple TheTriple(TT);

  // pass 0 instead of &_OS, if you do not want DWARF data to be forwarded to
  // the provided stream
  // this stream will be deleted in the destructor of BRIGAsmPrinter
  RawVectorOstream *rvos = new RawVectorOstream(&_OS);

  return createBRIGDwarfStreamer(Ctx, TAB, *rvos, _Emitter, RelaxAll,
                                 NoExecStack);
}

static MCStreamer *createMCStreamer(const Target &T, StringRef TT,
                                    MCContext &Ctx, MCAsmBackend &TAB,
                                    raw_ostream &_OS, MCCodeEmitter *_Emitter,
                                    const MCSubtargetInfo &MSI, bool RelaxAll,
                                    bool NoExecStack) {
  // FIXME: Create a stream for assembly printing.
  llvm_unreachable("Non-BRIG streamer not implemented");
}

extern "C" void LLVMInitializeHSAILTargetMC() {

  // Register the target asm info.
  RegisterMCAsmInfo<HSAILELFMCAsmInfo> A(TheHSAIL_32Target);
  RegisterMCAsmInfo<HSAILELFMCAsmInfo> B(TheHSAIL_64Target);

  RegisterMCCodeGenInfoFn C(TheHSAIL_32Target, createHSAILMCCodeGenInfo);
  RegisterMCCodeGenInfoFn D(TheHSAIL_64Target, createHSAILMCCodeGenInfo);

  TargetRegistry::RegisterMCCodeEmitter(TheHSAIL_32Target,
                                        createHSAIL_32MCCodeEmitterForLLVM32);
  TargetRegistry::RegisterMCAsmBackend(TheHSAIL_32Target,
                                       createHSAIL_32AsmBackendForLLVM32);
  TargetRegistry::RegisterMCObjectStreamer(TheHSAIL_32Target,
                                           createBRIGStreamer);

  TargetRegistry::RegisterMCCodeEmitter(TheHSAIL_64Target,
                                        createHSAIL_64MCCodeEmitterForLLVM32);
  TargetRegistry::RegisterMCAsmBackend(TheHSAIL_64Target,
                                       createHSAIL_64AsmBackendForLLVM32);
  TargetRegistry::RegisterMCObjectStreamer(TheHSAIL_64Target,
                                           createBRIGStreamer);
}
