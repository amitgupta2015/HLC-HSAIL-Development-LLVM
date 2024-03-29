//===-- HSAILTargetMachine.cpp - Define TargetMachine for the HSAIL -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the HSAIL specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#include "HSAILTargetMachine.h"
#include "HSAIL.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/PassManager.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Transforms/Scalar.h"

using namespace llvm;

static cl::opt<bool> DisableHSAILCFGOpts("disable-hsail-cfg-opts",
  cl::desc("Disable hsail control flow optimizations"));

extern llvm::cl::opt<TargetMachine::CodeGenFileType> FileType;

// TODO_HSA: As soon as -enable-experimetal llc option is not needed anymore
//           the code block below shall be removed.
namespace llvm {
  bool EnableExperimentalFeatures;

  cl::opt<bool,true> 
  EnableExperimental("enable-experimental",
    cl::desc("Enable experimental features"),
    cl::location(EnableExperimentalFeatures),
    cl::init(getenv("HSA_ENABLE_EXPERIMENTAL_FEATURES") && std::string(getenv("HSA_ENABLE_EXPERIMENTAL_FEATURES")) == std::string("1")));


  bool EnableUniformOperations;

  cl::opt<bool, true> EnableUniformOps("hsail-enable-uniform-ops", cl::Hidden,
    cl::desc("Enable width modifier for uniform operation"),
    cl::location(EnableUniformOperations),
    cl::init(true));

  enum OptimizeForTargetArch OptimizeFor;

  cl::opt<enum OptimizeForTargetArch, true> HsailOptimizeFor("hsail-optimize-for", cl::Hidden,
    cl::desc("Tune backend optimizations for specific target architecture"),
    cl::values(
      clEnumVal(GENERIC, "No target specific flavor"),
      clEnumVal(SI,      "Sea Islands family"),
    clEnumValEnd),
    cl::location(OptimizeFor),
    cl::init(SI));

  bool EnableGCNMinMax;

  cl::opt<bool, true> OptEnableGCNMinMax("hsail-enable-gcnminmax", cl::Hidden,
    cl::desc("Enable GCN min/max ops in tablegen Fusion"),
    cl::location(EnableGCNMinMax),
    cl::init(true));

}

extern "C" void LLVMInitializeHSAILTarget() {
  // Register the target.
  RegisterTargetMachine<HSAIL_32TargetMachine> X(TheHSAIL_32Target);
  RegisterTargetMachine<HSAIL_64TargetMachine> Y(TheHSAIL_64Target);
}


/// HSAILTargetMachine ctor -
///
HSAILTargetMachine::HSAILTargetMachine(const Target &T, StringRef TT,
    StringRef CPU, StringRef FS,const TargetOptions &Options, Reloc::Model RM, CodeModel::Model CM,CodeGenOpt::Level OL, bool is64bitTarget )
:
  LLVMTargetMachine(T, TT, CPU, FS,Options, RM, CM,OL),
  Subtarget(TT, CPU, FS, is64bitTarget, *this),
  //  DLInfo(Subtarget.getDataLayout()),
  //  InstrInfo(*this),
  //  TLInfo(*this), 
  IntrinsicInfo(this)
 {
     initAsmInfo();
     setAsmVerbosityDefault(true);

}

bool HSAILTargetMachine::addPassesToEmitFile(PassManagerBase &PM,
                                             formatted_raw_ostream &Out,
                                             CodeGenFileType FT,
                                             bool DisableVerify,
                                             AnalysisID StartAfter,
                                             AnalysisID StopAfter) {
  HSAILFileType = FT;

  // Use CGFT_ObjectFile regardless on the output format.
  // To process CGFT_AssemblyFile we will later disassemble generated BRIG.
  return LLVMTargetMachine::addPassesToEmitFile(PM, Out, CGFT_ObjectFile,
    DisableVerify, StartAfter, StopAfter);
}

TargetPassConfig*
HSAILTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new HSAILPassConfig(this, PM);
}

void HSAILPassConfig::addIRPasses() {
  // AddrSpaceCast optimization and lowering. Add dead code elimination
  // to eliminate dead instructions (AddrSpaceCast, etc.).

    addPass(createDeadCodeEliminationPass());

    TargetPassConfig::addIRPasses();
}

bool HSAILPassConfig::addPreISel(){
    addPass(createLCSSAPass()); // Required by early CFG opts

  return true;
}

bool HSAILPassConfig::addInstSelector() {
	HSAILTargetMachine &HSATM = getTM<HSAILTargetMachine>();
	//return HSAILTargetMachine::addInstSelector(*PM,HSATM.Options,HSATM.getOptLevel());
    //mOptLevel = OptLevel;
  // Install an instruction selector.

  addPass(createHSAILISelDag(HSATM, HSATM.getOptLevel()));

  return false;
}
bool HSAILPassConfig::addPreEmitPass()
{
	return false;
}
bool HSAILPassConfig::addPreRegAlloc()
{
	return false;
}
bool HSAILPassConfig::addPostRegAlloc()
{
	return false;
}

//===----------------------------------------------------------------------===//
// HSAIL_32Machine functions
//===----------------------------------------------------------------------===//
HSAIL_32TargetMachine::HSAIL_32TargetMachine(const Target &T, StringRef TT,
    StringRef CPU, StringRef FS,const TargetOptions &Options, Reloc::Model RM, CodeModel::Model CM,CodeGenOpt::Level OL)
  : HSAILTargetMachine(T, TT, CPU, FS,Options, RM, CM,OL, false /* is64bitTarget */),
    DLInfo("e-p:32:32-f64:64:64-i64:64:64-f80:32:32-f128:128:128-n32"),
    TSInfo(*this) {

        Triple TheTriple(TT);

        // Check for mismatch in target triple settings and data layout. Note the target
        // triple comes from the module (unless overridden on command line). It's just a
        // warning, but users should know if they're specifying --march=hsail-64 on a
        // 32-bit module or --march=hsail on a 64-bit module.
        if ( TheTriple.getArch() == Triple::hsail_64 ) {
          errs() << "warning: target triple '" << TT <<
              "' does not match target 'hsail', expecting hsail-pc-amdopencl.\n";
        }
}


//===----------------------------------------------------------------------===//
// HSAIL_64Machine functions
//===----------------------------------------------------------------------===//
HSAIL_64TargetMachine::HSAIL_64TargetMachine(const Target &T, StringRef TT,
    StringRef CPU, StringRef FS,const TargetOptions &Options, Reloc::Model RM, CodeModel::Model CM,CodeGenOpt::Level OL)
  : HSAILTargetMachine(T, TT, CPU, FS,Options, RM, CM,OL, true /* is64bitTarget */),
    DLInfo("e-p:64:64-s:64-f64:64:64-i64:64:64-f80:128:128-f128:128:128-n32:64"),
    TSInfo(*this) {

        Triple TheTriple(TT);

        // Check for mismatch in target triple settings and data layout. Note the target
        // triple comes from the module (unless overridden on command line). It's just a
        // warning, but users should know if they're specifying --march=hsail-64 on a
        // 32-bit module.
        if ( TheTriple.getArch() == Triple::hsail ) {
          errs() << "warning: target triple '" << TT <<
              "' does not match target 'hsail-64', expecting hsail64-pc-amdopencl.\n";
        }
}
