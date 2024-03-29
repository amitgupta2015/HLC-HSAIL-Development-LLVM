//===---- HSAILISelLowering.h - HSAIL DAG Lowering Interface ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes how to lower LLVM code to machine code.  This has two
// main components:
//
//  1. Which ValueTypes are natively supported by the target.
//  2. Which operations are supported for supported ValueTypes.
//  3. Cost thresholds for alternative implementations of certain operations.
//
// In addition it has a few other components, like information about FP
// immediates.
//
//===----------------------------------------------------------------------===//

#ifndef _HSAIL_LOWERING_H_
#define _HSAIL_LOWERING_H_

#include "HSAILRegisterInfo.h"
#include "HSAILMachineFunctionInfo.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Target/TargetLowering.h"
#include "HSAILISDNodes.h"

namespace llvm {

class HSAILSubtarget;

//===----------------------------------------------------------------------===//
/// HSAILLowering - This class defines information used to lower LLVM code to
/// legal SelectionDAG operators that the target instruction selector can accept
/// natively.
///
/// This class also defines callbacks that targets must implement to lower
/// target-specific constructs to SelectionDAG operators.
///
class HSAILTargetLowering : public TargetLowering {
private:
  /// Subtarget - Keep a pointer to the X86Subtarget around so that we can
  /// make the right decision when generating code for different targets.
  const HSAILSubtarget *Subtarget;
  const HSAILRegisterInfo *RegInfo;
  const DataLayout *DL;
public:
  /// NOTE: The constructor takes ownership of TLOF.
  explicit HSAILTargetLowering(HSAILTargetMachine &TM);
  virtual ~HSAILTargetLowering();

  /// getSetCCResultType - Return the ValueType of the result of SETCC
  /// operations.  Also used to obtain the target's preferred type for
  /// the condition operand of SELECT and BRCOND nodes.  In the case of
  /// BRCOND the argument passed is MVT::Other since there are no other
  /// operands to get a type hint from.
  virtual EVT
  getSetCCResultType(LLVMContext &Context, EVT VT) const;

  /// getCmpLibcallReturnType - Return the ValueType for comparison
  /// libcalls. Comparions libcalls include floating point comparion calls,
  /// and Ordered/Unordered check calls on floating point numbers.
  virtual MVT::SimpleValueType
  getCmpLibcallReturnType() const;

  /// getSchedulingPreference - Some scheduler, e.g. hybrid, can switch to
  /// different scheduling heuristics for different nodes. This function returns
  /// the preference (or none) for the given node.
  virtual Sched::Preference
  getSchedulingPreference(SDNode *N) const;

  /// getRepRegClassFor - Return the 'representative' register class for the
  /// specified value type. The 'representative' register class is the largest
  /// legal super-reg register class for the register class of the value type.
  /// For example, on i386 the rep register class for i8, i16, and i32 are GR32;
  /// while the rep register class is GR64 on x86_64.
  const TargetRegisterClass *getRepRegClassFor(MVT VT) const override;

  /// getRepRegClassCostFor - Return the cost of the 'representative' register
  /// class for the specified value type.
  uint8_t getRepRegClassCostFor(MVT VT) const override;

  /// getTgtMemIntrinsic: Given an intrinsic, checks if on the target the
  /// intrinsic will need to map to a MemIntrinsicNode (touches memory). If
  /// this is the case, it returns true and store the intrinsic
  /// information into the IntrinsicInfo that was passed to the function.
  virtual bool
  getTgtMemIntrinsic(IntrinsicInfo &Info,
                     const CallInst &I,
                     unsigned Intrinsic) const;

  /// isFPImmLegal - Returns true if the target can instruction select the
  /// specified FP immediate natively. If false, the legalizer will materialize
  /// the FP immediate as a load from a constant pool.
  virtual bool
  isFPImmLegal(const APFloat &Imm, EVT VT) const;

  /// isShuffleMaskLegal - Targets can use this to indicate that they only
  /// support *some* VECTOR_SHUFFLE operations, those with specific masks.
  /// By default, if a target supports the VECTOR_SHUFFLE node, all mask values
  /// are assumed to be legal.
  virtual bool
  isShuffleMaskLegal(const SmallVectorImpl<int> &Mask,
                     EVT VT) const;

  /// canOpTrap - Returns true if the operation can trap for the value type.
  /// VT must be a legal type. By default, we optimistically assume most
  /// operations don't trap except for divide and remainder.
  virtual bool
  canOpTrap(unsigned Op, EVT VT) const;

  /// isVectorClearMaskLegal - Similar to isShuffleMaskLegal. This is
  /// used by Targets can use this to indicate if there is a suitable
  /// VECTOR_SHUFFLE that can be used to replace a VAND with a constant
  /// pool entry.
  virtual bool
  isVectorClearMaskLegal(const SmallVectorImpl<int> &Mask,
                         EVT VT) const;

  /// getByValTypeAlignment - Return the desired alignment for ByVal aggregate
  /// function arguments in the caller parameter area.  This is the actual
  /// alignment, not its logarithm.
  virtual unsigned
  getByValTypeAlignment(Type *Ty) const;

  /// ShouldShrinkFPConstant - If true, then instruction selection should
  /// seek to shrink the FP constant of the specified type to a smaller type
  /// in order to save space and / or reduce runtime.
  virtual bool
  ShouldShrinkFPConstant(EVT VT) const;

  /// This function returns true if the target allows unaligned memory accesses.
  /// of the specified type. This is used, for example, in situations where an
  /// array copy/move/set is  converted to a sequence of store operations. It's
  /// use helps to ensure that such replacements don't generate code that causes
  /// an alignment error  (trap) on the target machine.
  /// @brief Determine if the target supports unaligned memory accesses.
  virtual bool
  allowsUnalignedMemoryAccesses(EVT VT) const;

  /// getPreIndexedAddressParts - returns true by value, base pointer and
  /// offset pointer and addressing mode by reference if the node's address
  /// can be legally represented as pre-indexed load / store address.
  virtual bool
  getPreIndexedAddressParts(SDNode *N,
                            SDValue &Base,
                            SDValue &Offset,
                            ISD::MemIndexedMode &AM,
                            SelectionDAG &DAG) const;

  /// getPostIndexedAddressParts - returns true by value, base pointer and
  /// offset pointer and addressing mode by reference if this node can be
  /// combined with a load / store to form a post-indexed load / store.
  virtual bool
  getPostIndexedAddressParts(SDNode *N,
                             SDNode *Op,
                             SDValue &Base,
                             SDValue &Offset,
                             ISD::MemIndexedMode &AM,
                             SelectionDAG &DAG) const;

  /// getJumpTableEncoding - Return the entry encoding for a jump table in the
  /// current function.  The returned value is a member of the
  /// MachineJumpTableInfo::JTEntryKind enum.
  virtual unsigned
  getJumpTableEncoding() const;

  virtual const MCExpr*
  LowerCustomJumpTableEntry(const MachineJumpTableInfo *MJTI,
                            const MachineBasicBlock *MBB,
                            unsigned uid,
                            MCContext &Ctx) const;

  /// getPICJumpTableRelocaBase - Returns relocation base for the given PIC
  /// jumptable.
  virtual SDValue
  getPICJumpTableRelocBase(SDValue Table,
                           SelectionDAG &DAG) const;

  /// getPICJumpTableRelocBaseExpr - This returns the relocation base for the
  /// given PIC jumptable, the same as getPICJumpTableRelocBase, but as an
  /// MCExpr.
  virtual const MCExpr*
  getPICJumpTableRelocBaseExpr(const MachineFunction *MF,
                               unsigned JTI,
                               MCContext &Ctx) const;

  /// isOffsetFoldingLegal - Return true if folding a constant offset
  /// with the given GlobalAddress is legal.  It is frequently not legal in
  /// PIC relocation models.
  virtual bool
  isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const;

  /// getFunctionAlignment - Return the Log2 alignment of this function.
  virtual unsigned
  getFunctionAlignment(const Function *) const;

  /// getStackCookieLocation - Return true if the target stores stack
  /// protector cookies at a fixed offset in some non-standard address
  /// space, and populates the address space and offset as
  /// appropriate.
  virtual bool
  getStackCookieLocation(unsigned &AddressSpace, unsigned &Offset) const;

  /// getMaximalGlobalOffset - Returns the maximal possible offset which can be
  /// used for loads / stores from the global.
  virtual unsigned
  getMaximalGlobalOffset() const;

  /// ComputeNumSignBitsForTargetNode - This method can be implemented by
  /// targets that want to expose additional information about sign bits to the
  /// DAG Combiner.
  unsigned ComputeNumSignBitsForTargetNode(SDValue Op,
                                           const SelectionDAG &DAG,
                                           unsigned Depth = 0) const override;

  /// isGAPlusOffset - Returns true (and the GlobalValue and the offset) if the
  /// node is a GlobalAddress + offset.
  virtual bool
  isGAPlusOffset(SDNode *N, const GlobalValue* &GA, int64_t &Offset) const;

  /// PerformDAGCombine - This method will be invoked for all target nodes and
  /// for any target-independent nodes that the target has registered with
  /// invoke it for.
  ///
  /// The semantics are as follows:
  /// Return Value:
  ///   SDValue.Val == 0   - No change was made
  ///   SDValue.Val == N   - N was replaced, is dead, and is already handled.
  ///   otherwise          - N should be replaced by the returned Operand.
  ///
  /// In addition, methods provided by DAGCombinerInfo may be used to perform
  /// more complex transformations.
  ///
  virtual SDValue
  PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const;

  /// isTypeDesirableForOp - Return true if the target has native support for
  /// the specified value type and it is 'desirable' to use the type for the
  /// given node type. e.g. On x86 i16 is legal, but undesirable since i16
  /// instruction encodings are longer and some i16 instructions are slow.
  virtual bool
  isTypeDesirableForOp(unsigned Opc, EVT VT) const;

  /// isDesirableToPromoteOp - Return true if it is profitable for dag combiner
  /// to transform a floating point op of specified opcode to a equivalent op of
  /// an integer type. e.g. f32 load -> i32 load can be profitable on ARM.
  virtual bool
  isDesirableToTransformToIntegerOp(unsigned Opc, EVT VT) const;

  /// IsDesirableToPromoteOp - This method query the target whether it is
  /// beneficial for dag combiner to promote the specified node. If true, it
  /// should return the desired promotion type by reference.
  virtual bool
  IsDesirableToPromoteOp(SDValue Op, EVT &PVT) const;

  /// Return true if the load uses larger data types than 
  /// the bitcast and false otherwise.
  /// This should disable optimizing:
  /// (char16)((int4*)ptr)[idx] => (char16*)ptr[idx]
  /// but not disable:
  /// (int4)((char16*)ptr)[idx] => (int4*)ptr[idx]
  bool
    isLoadBitCastBeneficial(EVT load, EVT bitcast) const;

  /// isVectorToScalarLoadStoreWidenBeneficial() - Return true if the vector
  /// load or store packing into a larger scalar type is beneficial.
  /// Width:     Width to load/store.
  /// WidenVT:   The widen vector type to load to/store from.
  /// N:         Load or store SDNode.
  virtual bool
  isVectorToScalarLoadStoreWidenBeneficial(unsigned Width, EVT WidenVT,
                                           const MemSDNode *N) const;

protected:

  /// Create kernel or function parameter scalar load and return its value.
  /// If isLoad = false create an argument value store.
  /// AddressSpace used to determine if that is a kernel or function argument.
  /// ArgVT specifies expected value type where 'Ty' refers to the real
  /// argument type from function's signature.
  /// We have to use machine nodes here because loads and stores must be glued
  /// together with the whole call sequence, while ISD::LOAD/STORE do not have
  /// a glue operand. That also skips instruction selection, so faster.
  /// If the call sequence is not glued we may have unrelated to call
  /// instructions scheduled into the argscope if intent was argscope use.
  /// This function inserts a load or store argument instruction with glue.
  /// If InFlag contains glue it is used for inbound glue. Glue is produced as a
  /// last result and can be consumed at will of the caller.
  SDValue getArgLoadOrStore(SelectionDAG &DAG, EVT ArgVT, Type *Ty, bool isLoad,
                            bool isSExt, unsigned AddressSpace,
                            SDValue Ptr, SDValue ParamValue,
                            unsigned index, SDLoc dl, SDValue Chain,
                            SDValue InFlag) const;

public:
  //===--------------------------------------------------------------------===//
  // Lowering methods - These methods must be implemented by targets so that
  // the SelectionDAGLowering code knows how to lower these.

  SDValue LowerMemArgument(SDValue Chain,
                           CallingConv::ID CallConv,
                           const SmallVectorImpl<ISD::InputArg> &ArgInfo,
                           SDLoc dl, SelectionDAG &DAG,
                           const CCValAssign &VA,  MachineFrameInfo *MFI,
                           unsigned i) const;


  // This method of the llvm::TargetLowering has been made virtual
  // under the AMD_HSAIL define to handle image/sampler object pointers
  // in a specific way.
  virtual EVT getValueType(Type *Ty, bool AllowUnknown = false) const;

  /// LowerFormalArguments - This hook must be implemented to lower the
  /// incoming (formal) arguments, described by the Ins array, into the
  /// specified DAG. The implementation should fill in the InVals array
  /// with legal-type argument values, and return the resulting token
  /// chain value.
  ///
  virtual SDValue
  LowerFormalArguments(SDValue Chain,
                       CallingConv::ID CallConv,
                       bool isVarArg,
                       const SmallVectorImpl<ISD::InputArg> &Ins,
                       SDLoc dl,
                       SelectionDAG &DAG,
                       SmallVectorImpl<SDValue> &InVals) const;

  /// LowerCall - This hook must be implemented to lower calls into the
  /// the specified DAG. The outgoing arguments to the call are described
  /// by the Outs array, and the values to be returned by the call are
  /// described by the Ins array. The implementation should fill in the
  /// InVals array with legal-type return values from the call, and return
  /// the resulting token chain value.
  virtual SDValue
  LowerCall(CallLoweringInfo &CLI,
                SmallVectorImpl<SDValue> &InVals) const override;

  /// LowerCallResult - Lower the result values of an ISD::CALL into the
  /// appropriate copies out of appropriate physical registers.  This assumes that
  /// Chain/InFlag are the input chain/flag to use, and that TheCall is the call
  /// being lowered.  The returns a SDNode with the same number of values as the
  /// ISD::CALL.
  SDValue LowerCallResult(SDValue Chain,
                          SDValue& InFlag,
                          const SmallVectorImpl<ISD::InputArg> &Ins,
                          Type *type,
                          SDLoc dl,
                          SelectionDAG &DAG,
                          SmallVectorImpl<SDValue> &InVals,
                          SDValue RetValue) const;
  /*
  /// CanLowerReturn - This hook should be implemented to check whether the
  /// return values described by the Outs array can fit into the return
  /// registers.  If false is returned, an sret-demotion is performed.
  ///
  virtual bool
  CanLowerReturn(CallingConv::ID CallConv,
                 bool isVarArg,
                 const SmallVectorImpl<ISD::OutputArg> &Outs,
                 LLVMContext &Context) const;
  */

  /// LowerReturn - This hook must be implemented to lower outgoing
  /// return values, described by the Outs array, into the specified
  /// DAG. The implementation should return the resulting token chain
  /// value.
  ///
  virtual SDValue
  LowerReturn(SDValue Chain,
              CallingConv::ID CallConv,
              bool isVarArg,
              const SmallVectorImpl<ISD::OutputArg> &Outs,
              const SmallVectorImpl<SDValue> &OutVals,
              SDLoc dl,
              SelectionDAG &DAG) const;

  /// LowerOperationWrapper - This callback is invoked by the type legalizer
  /// to legalize nodes with an illegal operand type but legal result types.
  /// It replaces the LowerOperation callback in the type Legalizer.
  /// The reason we can not do away with LowerOperation entirely is that
  /// LegalizeDAG isn't yet ready to use this callback.
  /// TODO: Consider merging with ReplaceNodeResults.

  /// The target places new result values for the node in Results (their number
  /// and types must exactly match those of the original return values of
  /// the node), or leaves Results empty, which indicates that the node is not
  /// to be custom lowered after all.
  /// The default implementation calls LowerOperation.
  virtual void
  LowerOperationWrapper(SDNode *N,
                        SmallVectorImpl<SDValue> &Results,
                        SelectionDAG &DAG) const;

  /// LowerOperation - This callback is invoked for operations that are
  /// unsupported by the target, which are registered to use 'custom' lowering,
  /// and whose defined values are all legal.
  /// If the target has no operations that require custom lowering, it need not
  /// implement this.  The default implementation of this aborts.
  virtual SDValue
  LowerOperation(SDValue Op, SelectionDAG &DAG) const;

  /// ReplaceNodeResults - This callback is invoked when a node result type is
  /// illegal for the target, and the operation was registered to use 'custom'
  /// lowering for that result type.  The target places new result values for
  /// the node in Results (their number and types must exactly match those of
  /// the original return values of the node), or leaves Results empty, which
  /// indicates that the node is not to be custom lowered after all.
  ///
  /// If the target has no operations that require custom lowering, it need not
  /// implement this.  The default implementation aborts.
  virtual void
  ReplaceNodeResults(SDNode *N,
                     SmallVectorImpl<SDValue> &Results,
                     SelectionDAG &DAG) const;

  /// getTargetNodeName() - This method returns the name of a target specific
  /// DAG node.
  virtual const char*
  getTargetNodeName(unsigned Opcode) const;

  /// Custom lowering methods
  SDValue 
  LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

  SDValue 
  LowerTRUNCATE(SDValue Op, SelectionDAG &DAG) const;

 SDValue
  LowerADD(SDValue Op, SelectionDAG &DAG) const;

  SDValue 
  LowerINTRINSIC_W_CHAIN(SDValue Op, SelectionDAG &DAG) const;

  SDValue 
  LowerROTL(SDValue Op, SelectionDAG &DAG) const;

  SDValue
  LowerROTR(SDValue Op, SelectionDAG &DAG) const;

  SDValue
  LowerBSWAP(SDValue Op, SelectionDAG &DAG) const;

  SDValue
  LowerLOAD(SDValue Op, SelectionDAG &DAG) const;

  SDValue
  LowerSTORE(SDValue Op, SelectionDAG &DAG) const;

  SDValue
  LowerATOMIC_LOAD(SDValue Op, SelectionDAG &DAG) const;

  SDValue
  LowerATOMIC_STORE(SDValue Op, SelectionDAG &DAG) const;

  //===--------------------------------------------------------------------===//
  // Instruction Emitting Hooks
  //

  //===--------------------------------------------------------------------===//
  // Addressing mode description hooks (used by LSR etc).
  //
  /// isLegalAddressingMode - Return true if the addressing mode represented by
  /// AM is legal for this target, for a load/store of the specified type.
  /// The type may be VoidTy, in which case only return true if the addressing
  /// mode is legal for a load/store of any legal type.
  /// TODO: Handle pre/postinc as well.
  bool isLegalAddressingMode(const AddrMode &AM, Type *Ty) const override;


  /// isTruncateFree - Return true if it's free to truncate a value of
  /// type Ty1 to type Ty2. e.g. On x86 it's free to truncate a i32 value in
  /// register EAX to i16 by referencing its sub-register AX.
  bool isTruncateFree(Type *Ty1, Type *Ty2) const override;

  bool isTruncateFree(EVT VT1, EVT VT2) const override;

  /// isZExtFree - Return true if any actual instruction that defines a
  /// value of type Ty1 implicitly zero-extends the value to Ty2 in the result
  /// register. This does not necessarily include registers defined in
  /// unknown ways, such as incoming arguments, or copies from unknown
  /// virtual registers. Also, if isTruncateFree(Ty2, Ty1) is true, this
  /// does not necessarily apply to truncate instructions. e.g. on x86-64,
  /// all instructions that define 32-bit values implicit zero-extend the
  /// result out to 64 bits.
  bool isZExtFree(Type *Ty1, Type *Ty2) const override;

  bool isZExtFree(EVT VT1, EVT VT2) const override;

  /// isNarrowingProfitable - Return true if it's profitable to narrow
  /// operations of type VT1 to VT2. e.g. on x86, it's profitable to narrow
  /// from i32 to i8 but not from i32 to i16.
  bool isNarrowingProfitable(EVT VT1, EVT VT2) const override;

  /// isLegalICmpImmediate - Return true if the specified immediate is legal
  /// icmp immediate, that is the target has icmp instructions which can compare
  /// a register against the immediate without having to materialize the
  /// immediate into a register.
  bool isLegalICmpImmediate(int64_t Imm) const override;

  MVT getScalarShiftAmountTy(EVT LHSTy) const override;

#if 0
  virtual void
  AdjustInstrPostInstrSelection(MachineInstr *MI, SDNode *Node) const;
#endif
  
};

} // end llvm namespace

#endif // _HSAIL_LOWERING_H_
