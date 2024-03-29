//===-- HSAILBrig.h - Definitions for BRIG files --------------------------===//
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

#ifndef HSAILBRIG_H_
#define HSAILBRIG_H_

typedef uint32_t BrigVersion32_t;

enum BrigVersion { //.nowrap //.nodump
    BRIG_VERSION_HSAIL_MAJOR = 0,
    BRIG_VERSION_HSAIL_MINOR = 20140528,
    BRIG_VERSION_BRIG_MAJOR = 0,
    BRIG_VERSION_BRIG_MINOR = 20140528
};

typedef uint8_t BrigAlignment8_t; //.defValue=Brig::BRIG_ALIGNMENT_NONE

typedef uint8_t BrigAllocation8_t; //.defValue=Brig::BRIG_ALLOCATION_NONE

typedef uint16_t BrigAluModifier16_t;

typedef uint8_t BrigAtomicOperation8_t;

typedef uint32_t BrigCodeOffset32_t; //.defValue=0 //.wtype=ItemRef<Code>

typedef uint8_t BrigCompareOperation8_t;

typedef uint16_t BrigControlDirective16_t;

typedef uint32_t BrigDataOffset32_t;

typedef BrigDataOffset32_t BrigDataOffsetCodeList32_t; //.wtype=ListRef<Code> //.defValue=0

typedef BrigDataOffset32_t BrigDataOffsetOperandList32_t; //.wtype=ListRef<Operand> //.defValue=0

typedef BrigDataOffset32_t BrigDataOffsetString32_t; //.wtype=StrRef //.defValue=0

typedef uint8_t BrigExecutableModifier8_t;

typedef uint8_t BrigImageChannelOrder8_t; //.defValue=Brig::BRIG_CHANNEL_ORDER_UNKNOWN

typedef uint8_t BrigImageChannelType8_t; //.defValue=Brig::BRIG_CHANNEL_TYPE_UNKNOWN

typedef uint8_t BrigImageGeometry8_t; //.defValue=Brig::BRIG_GEOMETRY_UNKNOWN

typedef uint8_t BrigImageQuery8_t;

typedef uint16_t BrigKinds16_t;

typedef uint8_t BrigLinkage8_t; //.defValue=Brig::BRIG_LINKAGE_NONE

typedef uint8_t BrigMachineModel8_t; //.defValue=Brig::BRIG_MACHINE_LARGE

typedef uint8_t BrigMemoryModifier8_t;

typedef uint8_t BrigMemoryOrder8_t; //.defValue=Brig::BRIG_MEMORY_ORDER_RELAXED

typedef uint8_t BrigMemoryScope8_t; //.defValue=Brig::BRIG_MEMORY_SCOPE_SYSTEM

typedef uint16_t BrigOpcode16_t;

typedef uint32_t BrigOperandOffset32_t; //.defValue=0 //.wtype=ItemRef<Operand>

typedef uint8_t BrigPack8_t; //.defValue=Brig::BRIG_PACK_NONE

typedef uint8_t BrigProfile8_t; //.defValue=Brig::BRIG_PROFILE_FULL

typedef uint16_t BrigRegisterKind16_t;

typedef uint8_t BrigRound8_t;

typedef uint8_t BrigSamplerAddressing8_t; //.defValue=Brig::BRIG_ADDRESSING_CLAMP_TO_EDGE

typedef uint8_t BrigSamplerCoordNormalization8_t;

typedef uint8_t BrigSamplerFilter8_t;

typedef uint8_t BrigSamplerQuery8_t;

typedef uint32_t BrigSectionIndex32_t;

typedef uint8_t BrigSegCvtModifier8_t;

typedef uint8_t BrigSegment8_t; //.defValue=Brig::BRIG_SEGMENT_NONE

typedef uint32_t BrigStringOffset32_t; //.defValue=0 //.wtype=StrRef

typedef uint16_t BrigType16_t;

typedef uint8_t BrigVariableModifier8_t;

typedef uint8_t BrigWidth8_t;

enum BrigKinds {
    //.wname={ s/^BRIG_KIND//; MACRO2Name($_) }
    //.mnemo=$wname{ $wname }
    //.sizeof=$wname{ "sizeof(Brig$wname)" }
    //.sizeof_switch //.sizeof_proto="int size_of_brig_record(unsigned arg)" //.sizeof_default="return -1"

    //.isBodyOnly={ "false" }
    //.isBodyOnly_switch //.isBodyOnly_proto="bool isBodyOnly(Directive d)" //.isBodyOnly_arg="d.brig()->kind"
    //.isBodyOnly_default="assert(false); return false"
    //.isToplevelOnly={ "false" }
    //.isToplevelOnly_switch //.isToplevelOnly_proto="bool isToplevelOnly(Directive d)" //.isToplevelOnly_arg="d.brig()->kind"
    //.isToplevelOnly_default="assert(false); return false"


    BRIG_KIND_NONE = 0x0000, //.skip

    BRIG_KIND_DIRECTIVE_BEGIN = 0x1000, //.skip
    BRIG_KIND_DIRECTIVE_ARG_BLOCK_END = 0x1000, //.isBodyOnly=true
    BRIG_KIND_DIRECTIVE_ARG_BLOCK_START = 0x1001, //.isBodyOnly=true
    BRIG_KIND_DIRECTIVE_COMMENT = 0x1002,
    BRIG_KIND_DIRECTIVE_CONTROL = 0x1003, //.isBodyOnly=true
    BRIG_KIND_DIRECTIVE_EXTENSION = 0x1004, //.isToplevelOnly=true
    BRIG_KIND_DIRECTIVE_FBARRIER = 0x1005,
    BRIG_KIND_DIRECTIVE_FUNCTION = 0x1006, //.isToplevelOnly=true
    BRIG_KIND_DIRECTIVE_INDIRECT_FUNCTION = 0x1007, //.isToplevelOnly=true
    BRIG_KIND_DIRECTIVE_KERNEL = 0x1008, //.isToplevelOnly=true
    BRIG_KIND_DIRECTIVE_LABEL = 0x1009, //.isBodyOnly=true
    BRIG_KIND_DIRECTIVE_LOC = 0x100a,
    BRIG_KIND_DIRECTIVE_PRAGMA = 0x100b,
    BRIG_KIND_DIRECTIVE_SIGNATURE = 0x100c, //.isToplevelOnly=true
    BRIG_KIND_DIRECTIVE_VARIABLE = 0x100d,
    BRIG_KIND_DIRECTIVE_VERSION = 0x100e, //.isToplevelOnly=true
    BRIG_KIND_DIRECTIVE_END = 0x100f, //.skip

    BRIG_KIND_INST_BEGIN = 0x2000, //.skip
    BRIG_KIND_INST_ADDR = 0x2000,
    BRIG_KIND_INST_ATOMIC = 0x2001,
    BRIG_KIND_INST_BASIC = 0x2002,
    BRIG_KIND_INST_BR = 0x2003,
    BRIG_KIND_INST_CMP = 0x2004,
    BRIG_KIND_INST_CVT = 0x2005,
    BRIG_KIND_INST_IMAGE = 0x2006,
    BRIG_KIND_INST_LANE = 0x2007,
    BRIG_KIND_INST_MEM = 0x2008,
    BRIG_KIND_INST_MEM_FENCE = 0x2009,
    BRIG_KIND_INST_MOD = 0x200a,
    BRIG_KIND_INST_QUERY_IMAGE = 0x200b,
    BRIG_KIND_INST_QUERY_SAMPLER = 0x200c,
    BRIG_KIND_INST_QUEUE = 0x200d,
    BRIG_KIND_INST_SEG = 0x200e,
    BRIG_KIND_INST_SEG_CVT = 0x200f,
    BRIG_KIND_INST_SIGNAL = 0x2010,
    BRIG_KIND_INST_SOURCE_TYPE = 0x2011,
    BRIG_KIND_INST_END = 0x2012, //.skip

    BRIG_KIND_OPERAND_BEGIN = 0x3000, //.skip
    BRIG_KIND_OPERAND_ADDRESS = 0x3000,
    BRIG_KIND_OPERAND_DATA = 0x3001,
    BRIG_KIND_OPERAND_CODE_LIST = 0x3002,
    BRIG_KIND_OPERAND_CODE_REF = 0x3003,
    BRIG_KIND_OPERAND_IMAGE_PROPERTIES = 0x3004,
    BRIG_KIND_OPERAND_OPERAND_LIST = 0x3005,
    BRIG_KIND_OPERAND_REG = 0x3006,
    BRIG_KIND_OPERAND_SAMPLER_PROPERTIES = 0x3007,
    BRIG_KIND_OPERAND_STRING = 0x3008,
    BRIG_KIND_OPERAND_WAVESIZE = 0x3009,
    BRIG_KIND_OPERAND_END = 0x300a //.skip
};

enum BrigAlignment {
    //.mnemo={ s/^BRIG_ALIGNMENT_//; lc }
    //.mnemo_proto="const char* align2str(unsigned arg)"

    //.bytes={ /(\d+)/ ? $1 : undef }
    //.bytes_switch //.bytes_proto="unsigned align2num(unsigned arg)" //.bytes_default="assert(false); return -1"

    //.rbytes=$bytes{ $bytes }
    //.rbytes_switch //.rbytes_reverse //.rbytes_proto="Brig::BrigAlignment num2align(uint64_t arg)"
    //.rbytes_default="return BRIG_ALIGNMENT_LAST"
    BRIG_ALIGNMENT_NONE = 0, //.no_mnemo
    BRIG_ALIGNMENT_1 = 1,  //.mnemo=""
    BRIG_ALIGNMENT_2 = 2,
    BRIG_ALIGNMENT_4 = 3,
    BRIG_ALIGNMENT_8 = 4,
    BRIG_ALIGNMENT_16 = 5,
    BRIG_ALIGNMENT_32 = 6,
    BRIG_ALIGNMENT_64 = 7,
    BRIG_ALIGNMENT_128 = 8,
    BRIG_ALIGNMENT_256 = 9,

    BRIG_ALIGNMENT_LAST, //.skip
    BRIG_ALIGNMENT_MAX = BRIG_ALIGNMENT_LAST - 1 //.skip
};

enum BrigAllocation {
    //.mnemo={ s/^BRIG_ALLOCATION_//;lc }
    //.mnemo_token=EAllocKind
    BRIG_ALLOCATION_NONE = 0, //.mnemo=""
    BRIG_ALLOCATION_PROGRAM = 1,
    BRIG_ALLOCATION_AGENT = 2,
    BRIG_ALLOCATION_AUTOMATIC = 3
};

enum BrigAluModifierMask {
    BRIG_ALU_ROUND = 31,
    BRIG_ALU_FTZ = 32
};

enum BrigAtomicOperation { //.tdcaption="Atomic Operations"
    //.mnemo={ s/^BRIG_ATOMIC_//;lc }
    //.mnemo_token=_EMAtomicOp
    //.mnemo_context=EInstModifierInstAtomicContext
    BRIG_ATOMIC_ADD = 0,
    BRIG_ATOMIC_AND = 1,
    BRIG_ATOMIC_CAS = 2,
    BRIG_ATOMIC_EXCH = 3,
    BRIG_ATOMIC_LD = 4,
    BRIG_ATOMIC_MAX = 5,
    BRIG_ATOMIC_MIN = 6,
    BRIG_ATOMIC_OR = 7,
    BRIG_ATOMIC_ST = 8,
    BRIG_ATOMIC_SUB = 9,
    BRIG_ATOMIC_WRAPDEC = 10,
    BRIG_ATOMIC_WRAPINC = 11,
    BRIG_ATOMIC_XOR = 12,
    BRIG_ATOMIC_WAIT_EQ = 13,
    BRIG_ATOMIC_WAIT_NE = 14,
    BRIG_ATOMIC_WAIT_LT = 15,
    BRIG_ATOMIC_WAIT_GTE = 16,
    BRIG_ATOMIC_WAITTIMEOUT_EQ = 17,
    BRIG_ATOMIC_WAITTIMEOUT_NE = 18,
    BRIG_ATOMIC_WAITTIMEOUT_LT = 19,
    BRIG_ATOMIC_WAITTIMEOUT_GTE = 20
};

enum BrigCompareOperation { //.tdcaption="Comparison Operators"
    //.mnemo={ s/^BRIG_COMPARE_//;lc }
    //.mnemo_token=_EMCompare
    BRIG_COMPARE_EQ = 0,
    BRIG_COMPARE_NE = 1,
    BRIG_COMPARE_LT = 2,
    BRIG_COMPARE_LE = 3,
    BRIG_COMPARE_GT = 4,
    BRIG_COMPARE_GE = 5,
    BRIG_COMPARE_EQU = 6,
    BRIG_COMPARE_NEU = 7,
    BRIG_COMPARE_LTU = 8,
    BRIG_COMPARE_LEU = 9,
    BRIG_COMPARE_GTU = 10,
    BRIG_COMPARE_GEU = 11,
    BRIG_COMPARE_NUM = 12,
    BRIG_COMPARE_NAN = 13,
    BRIG_COMPARE_SEQ = 14,
    BRIG_COMPARE_SNE = 15,
    BRIG_COMPARE_SLT = 16,
    BRIG_COMPARE_SLE = 17,
    BRIG_COMPARE_SGT = 18,
    BRIG_COMPARE_SGE = 19,
    BRIG_COMPARE_SGEU = 20,
    BRIG_COMPARE_SEQU = 21,
    BRIG_COMPARE_SNEU = 22,
    BRIG_COMPARE_SLTU = 23,
    BRIG_COMPARE_SLEU = 24,
    BRIG_COMPARE_SNUM = 25,
    BRIG_COMPARE_SNAN = 26,
    BRIG_COMPARE_SGTU = 27
};

enum BrigControlDirective {
    //.mnemo={ s/^BRIG_CONTROL_//;lc }
    //.mnemo_token=EControl
    BRIG_CONTROL_NONE = 0, //.skip
    BRIG_CONTROL_ENABLEBREAKEXCEPTIONS = 1,
    BRIG_CONTROL_ENABLEDETECTEXCEPTIONS = 2,
    BRIG_CONTROL_MAXDYNAMICGROUPSIZE = 3,
    BRIG_CONTROL_MAXFLATGRIDSIZE = 4,
    BRIG_CONTROL_MAXFLATWORKGROUPSIZE = 5,
    BRIG_CONTROL_REQUESTEDWORKGROUPSPERCU = 6,
    BRIG_CONTROL_REQUIREDDIM = 7,
    BRIG_CONTROL_REQUIREDGRIDSIZE = 8,
    BRIG_CONTROL_REQUIREDWORKGROUPSIZE = 9,
    BRIG_CONTROL_REQUIRENOPARTIALWORKGROUPS = 10
};

enum BrigExecutableModifierMask {
    //.nodump
    BRIG_EXECUTABLE_DEFINITION = 1
};

enum BrigImageChannelOrder {
    //.mnemo={ s/^BRIG_CHANNEL_ORDER_?//;lc }
    //.mnemo_token=EImageOrder
    //.mnemo_context=EImageOrderContext

    BRIG_CHANNEL_ORDER_A = 0,
    BRIG_CHANNEL_ORDER_R = 1,
    BRIG_CHANNEL_ORDER_RX = 2,
    BRIG_CHANNEL_ORDER_RG = 3,
    BRIG_CHANNEL_ORDER_RGX = 4,
    BRIG_CHANNEL_ORDER_RA = 5,
    BRIG_CHANNEL_ORDER_RGB = 6,
    BRIG_CHANNEL_ORDER_RGBX = 7,
    BRIG_CHANNEL_ORDER_RGBA = 8,
    BRIG_CHANNEL_ORDER_BGRA = 9,
    BRIG_CHANNEL_ORDER_ARGB = 10,
    BRIG_CHANNEL_ORDER_ABGR = 11,
    BRIG_CHANNEL_ORDER_SRGB = 12,
    BRIG_CHANNEL_ORDER_SRGBX = 13,
    BRIG_CHANNEL_ORDER_SRGBA = 14,
    BRIG_CHANNEL_ORDER_SBGRA = 15,
    BRIG_CHANNEL_ORDER_INTENSITY = 16,
    BRIG_CHANNEL_ORDER_LUMINANCE = 17,
    BRIG_CHANNEL_ORDER_DEPTH = 18,
    BRIG_CHANNEL_ORDER_DEPTH_STENCIL = 19,

    // used internally
    BRIG_CHANNEL_ORDER_UNKNOWN //.mnemo="" // used when no order is specified
};

enum BrigImageChannelType {
    //.mnemo={ s/^BRIG_CHANNEL_TYPE_//;lc }
    //.mnemo_token=EImageFormat

    BRIG_CHANNEL_TYPE_SNORM_INT8 = 0,
    BRIG_CHANNEL_TYPE_SNORM_INT16 = 1,
    BRIG_CHANNEL_TYPE_UNORM_INT8 = 2,
    BRIG_CHANNEL_TYPE_UNORM_INT16 = 3,
    BRIG_CHANNEL_TYPE_UNORM_INT24 = 4,
    BRIG_CHANNEL_TYPE_UNORM_SHORT_555 = 5,
    BRIG_CHANNEL_TYPE_UNORM_SHORT_565 = 6,
    BRIG_CHANNEL_TYPE_UNORM_INT_101010 = 7,
    BRIG_CHANNEL_TYPE_SIGNED_INT8 = 8,
    BRIG_CHANNEL_TYPE_SIGNED_INT16 = 9,
    BRIG_CHANNEL_TYPE_SIGNED_INT32 = 10,
    BRIG_CHANNEL_TYPE_UNSIGNED_INT8 = 11,
    BRIG_CHANNEL_TYPE_UNSIGNED_INT16 = 12,
    BRIG_CHANNEL_TYPE_UNSIGNED_INT32 = 13,
    BRIG_CHANNEL_TYPE_HALF_FLOAT = 14,
    BRIG_CHANNEL_TYPE_FLOAT = 15,

    // used internally
    BRIG_CHANNEL_TYPE_UNKNOWN //.mnemo=""
};

enum BrigImageGeometry { //.tdcaption="Geometry"
    //.mnemo={ s/^BRIG_GEOMETRY_//;lc }
    //.mnemo_token=EImageGeometry
    BRIG_GEOMETRY_1D = 0,
    BRIG_GEOMETRY_2D = 1,
    BRIG_GEOMETRY_3D = 2,
    BRIG_GEOMETRY_1DA = 3,
    BRIG_GEOMETRY_2DA = 4,
    BRIG_GEOMETRY_1DB = 5,
    BRIG_GEOMETRY_2DDEPTH = 6,
    BRIG_GEOMETRY_2DADEPTH = 7,

    // used internally
    BRIG_GEOMETRY_UNKNOWN //.mnemo=""
};

enum BrigImageQuery {
    //.mnemo={ s/^BRIG_IMAGE_QUERY_//;lc }
    BRIG_IMAGE_QUERY_WIDTH = 0,
    BRIG_IMAGE_QUERY_HEIGHT = 1,
    BRIG_IMAGE_QUERY_DEPTH = 2,
    BRIG_IMAGE_QUERY_ARRAY = 3,
    BRIG_IMAGE_QUERY_CHANNELORDER = 4,
    BRIG_IMAGE_QUERY_CHANNELTYPE = 5
};

enum BrigLinkage {
    //.mnemo={ s/^BRIG_LINKAGE_//;s/NONE//;lc }
    BRIG_LINKAGE_NONE = 0,
    BRIG_LINKAGE_PROGRAM = 1,
    BRIG_LINKAGE_MODULE = 2,
    BRIG_LINKAGE_FUNCTION = 3,
    BRIG_LINKAGE_ARG = 4
};

enum BrigMachineModel {
    //.mnemo={ s/^BRIG_MACHINE_//; '$'.lc }
    //.mnemo_token=ETargetMachine
    BRIG_MACHINE_SMALL = 0, // 32-bit model (all addresses are 32 bits;
    // a pointer fits into an s register)
    BRIG_MACHINE_LARGE = 1, // 64-bit model (all addresses are 64 bits;
    // a pointer fits into a d register)

    BRIG_MACHINE_UNDEF = 2 //.skip
};

enum BrigMemoryModifierMask {
    BRIG_MEMORY_CONST = 1
};

enum BrigMemoryOrder {
    //.mnemo={ s/^BRIG_MEMORY_ORDER_//; lc }
    //.mnemo_token=_EMMemoryOrder
    BRIG_MEMORY_ORDER_NONE = 0, //.mnemo=""
    BRIG_MEMORY_ORDER_RELAXED = 1, //.mnemo=rlx
    BRIG_MEMORY_ORDER_SC_ACQUIRE = 2, //.mnemo=scacq
    BRIG_MEMORY_ORDER_SC_RELEASE = 3, //.mnemo=screl
    BRIG_MEMORY_ORDER_SC_ACQUIRE_RELEASE = 4 //.mnemo=scar
};

enum BrigMemoryScope {
    //.mnemo={ s/^BRIG_MEMORY_SCOPE_//; lc }
    //.mnemo_token=_EMMemoryScope
    BRIG_MEMORY_SCOPE_NONE = 0, //.mnemo=""
    BRIG_MEMORY_SCOPE_WORKITEM = 1, //.mnemo=wi
    BRIG_MEMORY_SCOPE_WAVEFRONT = 2, //.mnemo=wv
    BRIG_MEMORY_SCOPE_WORKGROUP = 3, //.mnemo=wg
    BRIG_MEMORY_SCOPE_COMPONENT = 4, //.mnemo=cmp
    BRIG_MEMORY_SCOPE_SYSTEM = 5 //.mnemo=sys
};

enum BrigMemoryScope2 { // tbd HACK!
    //.mnemo={ s/^BRIG_MEMORY_SCOPE2_//; lc }
    //.mnemo_token=EMemoryScope
    //.mnemo_scanner=Instructions
    //.mnemo_context=EMemoryScopeContext
    BRIG_MEMORY_SCOPE2_NONE = 0, //.mnemo=""
    BRIG_MEMORY_SCOPE2_WORKITEM = 1, //.mnemo=wi
    BRIG_MEMORY_SCOPE2_WAVEFRONT = 2, //.mnemo=wv
    BRIG_MEMORY_SCOPE2_WORKGROUP = 3, //.mnemo=wg
    BRIG_MEMORY_SCOPE2_COMPONENT = 4, //.mnemo=cmp
    BRIG_MEMORY_SCOPE2_SYSTEM = 5 //.mnemo=sys
};

enum BrigOpcode { //.tdcaption="Instruction Opcodes"
    //.k={ "BASIC" }
    //.pscode=$k{ MACRO2Name("_".$k) }
    //.opcodeparser=$pscode{ return $pscode && "parseMnemo$pscode" }
    //.opcodeparser_incfile=ParserUtilities
    //.opcodeparser_switch //.opcodeparser_proto="OpcodeParser getOpcodeParser(Brig::BrigOpcode16_t arg)" //.opcodeparser_default="return parseMnemoBasic"

    //.psopnd={undef}
    //.opndparser=$psopnd{ return $psopnd && "&Parser::parse$psopnd" }
    //.opndparser_incfile=ParserUtilities
    //.opndparser_switch //.opndparser_proto="Parser::OperandParser Parser::getOperandParser(Brig::BrigOpcode16_t arg)" //.opndparser_default="return &Parser::parseOperands"

    //.mnemo={ s/^BRIG_OPCODE_//; s/GCN([^_])/GCN_$1/; lc }
    //.mnemo_scanner=Instructions //.mnemo_token=EInstruction
    //.mnemo_context=EDefaultContext

    //.has_memory_order={undef}
    //.semsupport=$has_memory_order{ return $has_memory_order && "true" }

    //.hasType=$k{ return ($k and $k eq "BASIC_NO_TYPE") ? "false" : undef; }
    //.hasType_switch //.hasType_proto="bool instHasType(Brig::BrigOpcode16_t arg)" //.hasType_default="return true"

    //.opcodevis=$pscode{ s/^BRIG_OPCODE_//; sprintf("%-47s(","vis.visitOpcode_".$_) . ($pscode =~m/^(BasicOrMod|Nop)$/? "inst" : "HSAIL_ASM::Inst". ($pscode=~m/BasicNoType/? "Basic":$pscode) ."(inst)").")" }
    //.opcodevis_switch //.opcodevis_proto="template <typename RetType, typename Visitor> RetType visitOpcode_gen(HSAIL_ASM::Inst inst, Visitor& vis)"
    //.opcodevis_arg="inst.opcode()" //.opcodevis_default="return RetType()"
    //.opcodevis_incfile=ItemUtils

    //.ftz=$k{ return ($k eq "BASIC_OR_MOD" or $k eq "CMP" or $k eq "CVT") ? "true" : undef }
    //.ftz_incfile=ItemUtils //.ftz_switch //.ftz_proto="inline bool instSupportsFtz(Brig::BrigOpcode16_t arg)" //.ftz_default="return false"

    //.vecOpndIndex={undef}
    //.vecOpndIndex_switch  //.vecOpndIndex_proto="int vecOpndIndex(Brig::BrigOpcode16_t arg)" //.vecOpndIndex_default="return -1"
    //.vecOpndIndex_incfile=ParserUtilities

    //.numdst={undef}
    //.numdst_switch //.numdst_proto="int instNumDstOperands(Brig::BrigOpcode16_t arg)" //.numdst_default="return 1"
    BRIG_OPCODE_NOP = 0, //.k=NOP //.hasType=false
    BRIG_OPCODE_ABS = 1, //.k=BASIC_OR_MOD
    BRIG_OPCODE_ADD = 2, //.k=BASIC_OR_MOD
    BRIG_OPCODE_BORROW = 3,
    BRIG_OPCODE_CARRY = 4,
    BRIG_OPCODE_CEIL = 5, //.k=BASIC_OR_MOD
    BRIG_OPCODE_COPYSIGN = 6, //.k=BASIC_OR_MOD
    BRIG_OPCODE_DIV = 7, //.k=BASIC_OR_MOD
    BRIG_OPCODE_FLOOR = 8, //.k=BASIC_OR_MOD
    BRIG_OPCODE_FMA = 9, //.k=BASIC_OR_MOD
    BRIG_OPCODE_FRACT = 10, //.k=BASIC_OR_MOD
    BRIG_OPCODE_MAD = 11,
    BRIG_OPCODE_MAX = 12, //.k=BASIC_OR_MOD
    BRIG_OPCODE_MIN = 13, //.k=BASIC_OR_MOD
    BRIG_OPCODE_MUL = 14, //.k=BASIC_OR_MOD
    BRIG_OPCODE_MULHI = 15, //.k=BASIC_OR_MOD
    BRIG_OPCODE_NEG = 16, //.k=BASIC_OR_MOD
    BRIG_OPCODE_REM = 17,
    BRIG_OPCODE_RINT = 18, //.k=BASIC_OR_MOD
    BRIG_OPCODE_SQRT = 19, //.k=BASIC_OR_MOD
    BRIG_OPCODE_SUB = 20, //.k=BASIC_OR_MOD
    BRIG_OPCODE_TRUNC = 21, //.k=BASIC_OR_MOD
    BRIG_OPCODE_MAD24 = 22,
    BRIG_OPCODE_MAD24HI = 23,
    BRIG_OPCODE_MUL24 = 24,
    BRIG_OPCODE_MUL24HI = 25,
    BRIG_OPCODE_SHL = 26,
    BRIG_OPCODE_SHR = 27,
    BRIG_OPCODE_AND = 28,
    BRIG_OPCODE_NOT = 29,
    BRIG_OPCODE_OR = 30,
    BRIG_OPCODE_POPCOUNT = 31, //.k=SOURCE_TYPE
    BRIG_OPCODE_XOR = 32,
    BRIG_OPCODE_BITEXTRACT = 33,
    BRIG_OPCODE_BITINSERT = 34,
    BRIG_OPCODE_BITMASK = 35,
    BRIG_OPCODE_BITREV = 36,
    BRIG_OPCODE_BITSELECT = 37,
    BRIG_OPCODE_FIRSTBIT = 38, //.k=SOURCE_TYPE
    BRIG_OPCODE_LASTBIT = 39, //.k=SOURCE_TYPE
    BRIG_OPCODE_COMBINE = 40, //.k=SOURCE_TYPE //.vecOpndIndex=1
    BRIG_OPCODE_EXPAND = 41, //.k=SOURCE_TYPE //.vecOpndIndex=0
    BRIG_OPCODE_LDA = 42, //.k=ADDR
    BRIG_OPCODE_MOV = 43,
    BRIG_OPCODE_SHUFFLE = 44,
    BRIG_OPCODE_UNPACKHI = 45,
    BRIG_OPCODE_UNPACKLO = 46,
    BRIG_OPCODE_PACK = 47, //.k=SOURCE_TYPE
    BRIG_OPCODE_UNPACK = 48, //.k=SOURCE_TYPE
    BRIG_OPCODE_CMOV = 49,
    BRIG_OPCODE_CLASS = 50, //.k=SOURCE_TYPE
    BRIG_OPCODE_NCOS = 51,
    BRIG_OPCODE_NEXP2 = 52,
    BRIG_OPCODE_NFMA = 53,
    BRIG_OPCODE_NLOG2 = 54,
    BRIG_OPCODE_NRCP = 55,
    BRIG_OPCODE_NRSQRT = 56,
    BRIG_OPCODE_NSIN = 57,
    BRIG_OPCODE_NSQRT = 58,
    BRIG_OPCODE_BITALIGN = 59,
    BRIG_OPCODE_BYTEALIGN = 60,
    BRIG_OPCODE_PACKCVT = 61, //.k=SOURCE_TYPE
    BRIG_OPCODE_UNPACKCVT = 62, //.k=SOURCE_TYPE
    BRIG_OPCODE_LERP = 63,
    BRIG_OPCODE_SAD = 64, //.k=SOURCE_TYPE
    BRIG_OPCODE_SADHI = 65, //.k=SOURCE_TYPE
    BRIG_OPCODE_SEGMENTP = 66, //.k=SEG_CVT
    BRIG_OPCODE_FTOS = 67, //.k=SEG_CVT
    BRIG_OPCODE_STOF = 68, //.k=SEG_CVT
    BRIG_OPCODE_CMP = 69, //.k=CMP
    BRIG_OPCODE_CVT = 70, //.k=CVT
    BRIG_OPCODE_LD = 71, //.k=MEM //.has_memory_order //.vecOpndIndex=0
    BRIG_OPCODE_ST = 72, //.k=MEM //.has_memory_order //.vecOpndIndex=0 //.numdst=0
    BRIG_OPCODE_ATOMIC = 73, //.k=ATOMIC
    BRIG_OPCODE_ATOMICNORET = 74, //.k=ATOMIC //.numdst=0
    BRIG_OPCODE_SIGNAL = 75, //.k=SIGNAL
    BRIG_OPCODE_SIGNALNORET = 76, //.k=SIGNAL //.numdst=0
    BRIG_OPCODE_MEMFENCE = 77, //.k=MEM_FENCE //.numdst=0
    BRIG_OPCODE_RDIMAGE = 78, //.k=IMAGE //.vecOpndIndex=0
    BRIG_OPCODE_LDIMAGE = 79, //.k=IMAGE //.vecOpndIndex=0
    BRIG_OPCODE_STIMAGE = 80, //.k=IMAGE //.vecOpndIndex=0 //.numdst=0
    BRIG_OPCODE_QUERYIMAGE = 81, //.k=QUERY_IMAGE
    BRIG_OPCODE_QUERYSAMPLER = 82, //.k=QUERY_SAMPLER
    BRIG_OPCODE_CBR = 83, //.k=BR //.numdst=0
    BRIG_OPCODE_BR = 84, //.k=BR //.numdst=0 //.hasType=false
    BRIG_OPCODE_SBR = 85,    //.k=BR //.numdst=0 //.psopnd=SbrOperands
    BRIG_OPCODE_BARRIER = 86, //.k=BR  //.numdst=0 //.hasType=false
    BRIG_OPCODE_WAVEBARRIER = 87, //.k=BR //.numdst=0 //.hasType=false
    BRIG_OPCODE_ARRIVEFBAR = 88, //.k=BR //.numdst=0 //.hasType=false
    BRIG_OPCODE_INITFBAR = 89, //.k=BASIC_NO_TYPE //.numdst=0 //.hasType=false
    BRIG_OPCODE_JOINFBAR = 90, //.k=BR //.numdst=0 //.hasType=false
    BRIG_OPCODE_LEAVEFBAR = 91, //.k=BR //.numdst=0 //.hasType=false
    BRIG_OPCODE_RELEASEFBAR = 92, //.k=BASIC_NO_TYPE //.numdst=0
    BRIG_OPCODE_WAITFBAR = 93, //.k=BR //.numdst=0 //.hasType=false
    BRIG_OPCODE_LDF = 94,
    BRIG_OPCODE_ACTIVELANECOUNT = 95, //.k=LANE
    BRIG_OPCODE_ACTIVELANEID = 96, //.k=LANE
    BRIG_OPCODE_ACTIVELANEMASK = 97, //.k=LANE //.vecOpndIndex=0
    BRIG_OPCODE_ACTIVELANESHUFFLE = 98, //.k=LANE
    BRIG_OPCODE_CALL = 99, //.k=BR //.psopnd=CallOperands //.numdst=0 //.hasType=false
    BRIG_OPCODE_SCALL = 100,  //.k=BR //.psopnd=CallOperands //.numdst=0
    BRIG_OPCODE_ICALL = 101, //.k=BR //.psopnd=CallOperands //.numdst=0
    BRIG_OPCODE_LDI = 102,
    BRIG_OPCODE_RET = 103, //.k=BASIC_NO_TYPE
    BRIG_OPCODE_ALLOCA = 104, //.k=MEM
    BRIG_OPCODE_CURRENTWORKGROUPSIZE = 105,
    BRIG_OPCODE_DIM = 106,
    BRIG_OPCODE_GRIDGROUPS = 107,
    BRIG_OPCODE_GRIDSIZE = 108,
    BRIG_OPCODE_PACKETCOMPLETIONSIG = 109,
    BRIG_OPCODE_PACKETID = 110,
    BRIG_OPCODE_WORKGROUPID = 111,
    BRIG_OPCODE_WORKGROUPSIZE = 112,
    BRIG_OPCODE_WORKITEMABSID = 113,
    BRIG_OPCODE_WORKITEMFLATABSID = 114,
    BRIG_OPCODE_WORKITEMFLATID = 115,
    BRIG_OPCODE_WORKITEMID = 116,
    BRIG_OPCODE_CLEARDETECTEXCEPT = 117,  //.numdst=0
    BRIG_OPCODE_GETDETECTEXCEPT = 118,
    BRIG_OPCODE_SETDETECTEXCEPT = 119,  //.numdst=0
    BRIG_OPCODE_ADDQUEUEWRITEINDEX = 120, //.k=QUEUE
    BRIG_OPCODE_AGENTCOUNT = 121,
    BRIG_OPCODE_AGENTID = 122,
    BRIG_OPCODE_CASQUEUEWRITEINDEX = 123, //.k=QUEUE
    BRIG_OPCODE_LDK = 124,
    BRIG_OPCODE_LDQUEUEREADINDEX = 125, //.k=QUEUE
    BRIG_OPCODE_LDQUEUEWRITEINDEX = 126, //.k=QUEUE
    BRIG_OPCODE_QUEUEID = 127,
    BRIG_OPCODE_QUEUEPTR = 128,
    BRIG_OPCODE_STQUEUEREADINDEX = 129, //.k=QUEUE //.numdst=0
    BRIG_OPCODE_STQUEUEWRITEINDEX = 130, //.k=QUEUE //.numdst=0
    BRIG_OPCODE_CLOCK = 131,
    BRIG_OPCODE_CUID = 132,
    BRIG_OPCODE_DEBUGTRAP = 133,  //.numdst=0
    BRIG_OPCODE_GROUPBASEPTR = 134,
    BRIG_OPCODE_KERNARGBASEPTR = 135,
    BRIG_OPCODE_LANEID = 136,
    BRIG_OPCODE_MAXCUID = 137,
    BRIG_OPCODE_MAXWAVEID = 138,
    BRIG_OPCODE_NULLPTR = 139, //.k=SEG
    BRIG_OPCODE_WAVEID = 140,

    BRIG_OPCODE_GCNMADU = (1u << 15) | 0, //.k=BASIC_NO_TYPE
    BRIG_OPCODE_GCNMADS = (1u << 15) | 1, //.k=BASIC_NO_TYPE
    BRIG_OPCODE_GCNMAX3 = (1u << 15) | 2,
    BRIG_OPCODE_GCNMIN3 = (1u << 15) | 3,
    BRIG_OPCODE_GCNMED3 = (1u << 15) | 4,
    BRIG_OPCODE_GCNFLDEXP = (1u << 15) | 5,
    BRIG_OPCODE_GCNFREXP_EXP = (1u << 15) | 6,
    BRIG_OPCODE_GCNFREXP_MANT = (1u << 15) | 7,
    BRIG_OPCODE_GCNTRIG_PREOP = (1u << 15) | 8,
    BRIG_OPCODE_GCNBFM = (1u << 15) | 9,
    BRIG_OPCODE_GCNLD = (1u << 15) | 10, //.k=MEM //.has_memory_order //.vecOpndIndex=0
    BRIG_OPCODE_GCNST = (1u << 15) | 11, //.k=MEM //.has_memory_order //.vecOpndIndex=0
    BRIG_OPCODE_GCNATOMIC = (1u << 15) | 12, //.k=ATOMIC
    BRIG_OPCODE_GCNATOMICNORET = (1u << 15) | 13, //.k=ATOMIC //.mnemo=gcn_atomicNoRet
    BRIG_OPCODE_GCNSLEEP = (1u << 15) | 14,
    BRIG_OPCODE_GCNPRIORITY = (1u << 15) | 15,
    BRIG_OPCODE_GCNREGIONALLOC = (1u << 15) | 16, //.k=BASIC_NO_TYPE //.mnemo=gcn_region_alloc
    BRIG_OPCODE_GCNMSAD = (1u << 15) | 17,
    BRIG_OPCODE_GCNQSAD = (1u << 15) | 18,
    BRIG_OPCODE_GCNMQSAD = (1u << 15) | 19,
    BRIG_OPCODE_GCNMQSAD4 = (1u << 15) | 20, //.k=BASIC_NO_TYPE
    BRIG_OPCODE_GCNSADW = (1u << 15) | 21,
    BRIG_OPCODE_GCNSADD = (1u << 15) | 22,
    BRIG_OPCODE_GCNCONSUME = (1u << 15) | 23, //.k=ADDR //.mnemo=gcn_atomic_consume
    BRIG_OPCODE_GCNAPPEND = (1u << 15) | 24, //.k=ADDR //.mnemo=gcn_atomic_append
    BRIG_OPCODE_GCNB4XCHG = (1u << 15) | 25, //.mnemo=gcn_b4xchg
    BRIG_OPCODE_GCNB32XCHG = (1u << 15) | 26, //.mnemo=gcn_b32xchg
    BRIG_OPCODE_GCNMAX = (1u << 15) | 27,
    BRIG_OPCODE_GCNMIN = (1u << 15) | 28,
};

enum BrigPack { //.tdcaption="Packing"
    //.mnemo={ s/^BRIG_PACK_//;s/SAT$/_sat/;lc }
    //.mnemo_token=_EMPacking
    BRIG_PACK_NONE = 0, //.mnemo=""
    BRIG_PACK_PP = 1,
    BRIG_PACK_PS = 2,
    BRIG_PACK_SP = 3,
    BRIG_PACK_SS = 4,
    BRIG_PACK_S = 5,
    BRIG_PACK_P = 6,
    BRIG_PACK_PPSAT = 7,
    BRIG_PACK_PSSAT = 8,
    BRIG_PACK_SPSAT = 9,
    BRIG_PACK_SSSAT = 10,
    BRIG_PACK_SSAT = 11,
    BRIG_PACK_PSAT = 12
};

enum BrigProfile {
    //.mnemo={ s/^BRIG_PROFILE_//;'$'.lc }
    //.mnemo_token=ETargetProfile
    BRIG_PROFILE_BASE = 0,
    BRIG_PROFILE_FULL = 1,

    BRIG_PROFILE_UNDEF = 2 //.skip
};

enum BrigRegisterKind {
    //.mnemo={ s/^BRIG_REGISTER_//;'$'.lc(substr($_,0,1)) }

    //.bits={ }
    //.bits_switch //.bits_proto="unsigned getRegBits(Brig::BrigRegisterKind16_t arg)" //.bits_default="return (unsigned)-1"
    BRIG_REGISTER_CONTROL = 0, //.bits=1
    BRIG_REGISTER_SINGLE = 1,  //.bits=32
    BRIG_REGISTER_DOUBLE = 2,  //.bits=64
    BRIG_REGISTER_QUAD = 3     //.bits=128
};

enum BrigRound {
    //.mnemo
    //.mnemo_fn=round2str //.mnemo_token=_EMRound
    BRIG_ROUND_NONE = 0,
    BRIG_ROUND_FLOAT_NEAR_EVEN = 1, //.mnemo=near
    BRIG_ROUND_FLOAT_ZERO = 2, //.mnemo=zero
    BRIG_ROUND_FLOAT_PLUS_INFINITY = 3, //.mnemo=up
    BRIG_ROUND_FLOAT_MINUS_INFINITY = 4, //.mnemo=down
    BRIG_ROUND_INTEGER_NEAR_EVEN = 5, //.mnemo=neari
    BRIG_ROUND_INTEGER_ZERO = 6, //.mnemo=zeroi
    BRIG_ROUND_INTEGER_PLUS_INFINITY = 7, //.mnemo=upi
    BRIG_ROUND_INTEGER_MINUS_INFINITY = 8, //.mnemo=downi
    BRIG_ROUND_INTEGER_NEAR_EVEN_SAT = 9, //.mnemo=neari_sat
    BRIG_ROUND_INTEGER_ZERO_SAT = 10, //.mnemo=zeroi_sat
    BRIG_ROUND_INTEGER_PLUS_INFINITY_SAT = 11, //.mnemo=upi_sat
    BRIG_ROUND_INTEGER_MINUS_INFINITY_SAT = 12, //.mnemo=downi_sat
    BRIG_ROUND_INTEGER_SIGNALLING_NEAR_EVEN = 13, //.mnemo=sneari
    BRIG_ROUND_INTEGER_SIGNALLING_ZERO = 14, //.mnemo=szeroi
    BRIG_ROUND_INTEGER_SIGNALLING_PLUS_INFINITY = 15, //.mnemo=supi
    BRIG_ROUND_INTEGER_SIGNALLING_MINUS_INFINITY = 16, //.mnemo=sdowni
    BRIG_ROUND_INTEGER_SIGNALLING_NEAR_EVEN_SAT = 17, //.mnemo=sneari_sat
    BRIG_ROUND_INTEGER_SIGNALLING_ZERO_SAT = 18, //.mnemo=szeroi_sat
    BRIG_ROUND_INTEGER_SIGNALLING_PLUS_INFINITY_SAT = 19, //.mnemo=supi_sat
    BRIG_ROUND_INTEGER_SIGNALLING_MINUS_INFINITY_SAT = 20 //.mnemo=sdowni_sat
};

enum BrigSamplerAddressing {
    //.mnemo={ s/^BRIG_ADDRESSING_//;lc }
    //.mnemo_token=ESamplerAddressingMode
    BRIG_ADDRESSING_UNDEFINED = 0,
    BRIG_ADDRESSING_CLAMP_TO_EDGE = 1,
    BRIG_ADDRESSING_CLAMP_TO_BORDER = 2,
    BRIG_ADDRESSING_REPEAT = 3,
    BRIG_ADDRESSING_MIRRORED_REPEAT = 4
};

enum BrigSamplerCoordNormalization {
    //.mnemo={ s/^BRIG_COORD_//;lc }
    //.mnemo_token=ESamplerCoord
    BRIG_COORD_UNNORMALIZED = 0,
    BRIG_COORD_NORMALIZED = 1
};

enum BrigSamplerFilter {
    //.mnemo={ s/^BRIG_FILTER_//;lc }
    BRIG_FILTER_NEAREST = 0,
    BRIG_FILTER_LINEAR = 1
};

enum BrigSamplerQuery {
    //.mnemo={ s/^BRIG_SAMPLER_QUERY_//;lc }
    //.mnemo_token=_EMSamplerQuery
    BRIG_SAMPLER_QUERY_ADDRESSING = 0,
    BRIG_SAMPLER_QUERY_COORD = 1,
    BRIG_SAMPLER_QUERY_FILTER = 2
};

enum BrigSectionIndex {
    //.mnemo={ s/^BRIG_SECTION_INDEX_/HSA_/;lc }
    BRIG_SECTION_INDEX_DATA = 0,
    BRIG_SECTION_INDEX_CODE = 1,
    BRIG_SECTION_INDEX_OPERAND = 2,
    BRIG_SECTION_INDEX_BEGIN_IMPLEMENTATION_DEFINED = 3,

    // used internally
    BRIG_SECTION_INDEX_IMPLEMENTATION_DEFINED = BRIG_SECTION_INDEX_BEGIN_IMPLEMENTATION_DEFINED //.skip
};

enum BrigSegCvtModifierMask {
    BRIG_SEG_CVT_NONULL = 1 //.mnemo="nonull"
};

enum BrigSegment {
    //.mnemo={ s/^BRIG_SEGMENT_//;lc}
    //.mnemo_token=_EMSegment
    //.mnemo_context=EInstModifierContext
    BRIG_SEGMENT_NONE = 0, //.mnemo=""
    BRIG_SEGMENT_FLAT = 1, //.mnemo=""
    BRIG_SEGMENT_GLOBAL = 2,
    BRIG_SEGMENT_READONLY = 3,
    BRIG_SEGMENT_KERNARG = 4,
    BRIG_SEGMENT_GROUP = 5,
    BRIG_SEGMENT_PRIVATE = 6,
    BRIG_SEGMENT_SPILL = 7,
    BRIG_SEGMENT_ARG = 8,
    BRIG_SEGMENT_EXTSPACE0 = 9, //.mnemo="region"
};

enum BrigPackedTypeBits {
    //.nodump
    BRIG_TYPE_PACK_SHIFT = 5,
    BRIG_TYPE_BASE_MASK = (1 << BRIG_TYPE_PACK_SHIFT) - 1,
    BRIG_TYPE_PACK_MASK = 3 << BRIG_TYPE_PACK_SHIFT,
    BRIG_TYPE_PACK_NONE = 0 << BRIG_TYPE_PACK_SHIFT,
    BRIG_TYPE_PACK_32 = 1 << BRIG_TYPE_PACK_SHIFT,
    BRIG_TYPE_PACK_64 = 2 << BRIG_TYPE_PACK_SHIFT,
    BRIG_TYPE_PACK_128 = 3 << BRIG_TYPE_PACK_SHIFT
};


enum BrigTypeX {
    //.numBits={ /([0-9]+)X([0-9]+)/ ? $1*$2 : /([0-9]+)/ ? $1 : undef }
    //.numBits_switch //.numBits_proto="unsigned getBrigTypeNumBits(unsigned arg)" //.numBits_default="assert(0); return 0"
    //.numBytes=$numBits{ $numBits > 1 ? $numBits/8 : undef }
    //.numBytes_switch //.numBytes_proto="unsigned getBrigTypeNumBytes(unsigned arg)" //.numBytes_default="assert(0); return 0"
    //.mnemo={ s/^BRIG_TYPE_//;lc }
    //.mnemo_token=_EMType
    //.dispatch_switch //.dispatch_incfile=TemplateUtilities
    //.dispatch_proto="template<typename RetType, typename Visitor>\nRetType dispatchByType_gen(unsigned type, Visitor& v)"
    //.dispatch={ /^BRIG_TYPE_([BUSF]|SIG)[0-9]+/ ? "v.template visit< BrigType<$_> >()" : "v.visitNone(type)" }
    //.dispatch_arg="type" //.dispatch_default="return v.visitNone(type)"
    BRIG_TYPE_NONE = 0, //.mnemo=""
    BRIG_TYPE_U8 = 1, //.ctype=uint8_t // unsigned integer 8 bits
    BRIG_TYPE_U16 = 2, //.ctype=uint16_t // unsigned integer 16 bits
    BRIG_TYPE_U32 = 3, //.ctype=uint32_t // unsigned integer 32 bits
    BRIG_TYPE_U64 = 4, //.ctype=uint64_t // unsigned integer 64 bits
    BRIG_TYPE_S8 = 5, //.ctype=int8_t // signed integer 8 bits
    BRIG_TYPE_S16 = 6, //.ctype=int16_t // signed integer 16 bits
    BRIG_TYPE_S32 = 7, //.ctype=int32_t // signed integer 32 bits
    BRIG_TYPE_S64 = 8, //.ctype=int64_t // signed integer 64 bits
    BRIG_TYPE_F16 = 9, //.ctype=f16_t // floating-point 16 bits
    BRIG_TYPE_F32 = 10, //.ctype=float // floating-point 32 bits
    BRIG_TYPE_F64 = 11, //.ctype=double // floating-point 64 bits
    BRIG_TYPE_B1 = 12, //.ctype=bool //.numBytes=1 // uninterpreted bit string of length 1 bit
    BRIG_TYPE_B8 = 13, //.ctype=uint8_t // uninterpreted bit string of length 8 bits
    BRIG_TYPE_B16 = 14, //.ctype=uint16_t // uninterpreted bit string of length 16 bits
    BRIG_TYPE_B32 = 15, //.ctype=uint32_t // uninterpreted bit string of length 32 bits
    BRIG_TYPE_B64 = 16, //.ctype=uint64_t // uninterpreted bit string of length 64 bits
    BRIG_TYPE_B128 = 17, //.ctype=b128_t // uninterpreted bit string of length 128 bits
    BRIG_TYPE_SAMP = 18, //.mnemo=samp //.numBits=64 // sampler object
    BRIG_TYPE_ROIMG = 19, //.mnemo=roimg //.numBits=64 // read-only image object
    BRIG_TYPE_WOIMG = 20, //.mnemo=woimg //.numBits=64 // write-only image object
    BRIG_TYPE_RWIMG = 21, //.mnemo=rwimg //.numBits=64 // read/write image object
    BRIG_TYPE_SIG32 = 22, //.mnemo=sig32 //.numBits=64 // signal handle to signal object with 32 bit signal value
    BRIG_TYPE_SIG64 = 23, //.mnemo=sig64 //.numBits=64 // signal handle to signal object with 64 bit signal value


    BRIG_TYPE_U8X4 = BRIG_TYPE_U8 | BRIG_TYPE_PACK_32, //.ctype=uint8_t // four bytes unsigned
    BRIG_TYPE_U8X8 = BRIG_TYPE_U8 | BRIG_TYPE_PACK_64, //.ctype=uint8_t // eight bytes unsigned
    BRIG_TYPE_U8X16 = BRIG_TYPE_U8 | BRIG_TYPE_PACK_128, //.ctype=uint8_t // 16 bytes unsigned
    BRIG_TYPE_U16X2 = BRIG_TYPE_U16 | BRIG_TYPE_PACK_32, //.ctype=uint16_t // two short unsigned integers
    BRIG_TYPE_U16X4 = BRIG_TYPE_U16 | BRIG_TYPE_PACK_64, //.ctype=uint16_t // four short unsigned integers
    BRIG_TYPE_U16X8 = BRIG_TYPE_U16 | BRIG_TYPE_PACK_128, //.ctype=uint16_t // eight short unsigned integer
    BRIG_TYPE_U32X2 = BRIG_TYPE_U32 | BRIG_TYPE_PACK_64, //.ctype=uint32_t // two unsigned integers
    BRIG_TYPE_U32X4 = BRIG_TYPE_U32 | BRIG_TYPE_PACK_128, //.ctype=uint32_t // four unsigned integers
    BRIG_TYPE_U64X2 = BRIG_TYPE_U64 | BRIG_TYPE_PACK_128, //.ctype=uint64_t // two 64-bit unsigned integers
    BRIG_TYPE_S8X4 = BRIG_TYPE_S8 | BRIG_TYPE_PACK_32, //.ctype=int8_t // four bytes signed
    BRIG_TYPE_S8X8 = BRIG_TYPE_S8 | BRIG_TYPE_PACK_64, //.ctype=int8_t // eight bytes signed
    BRIG_TYPE_S8X16 = BRIG_TYPE_S8 | BRIG_TYPE_PACK_128, //.ctype=int8_t // 16 bytes signed
    BRIG_TYPE_S16X2 = BRIG_TYPE_S16 | BRIG_TYPE_PACK_32, //.ctype=int16_t // two short signed integers
    BRIG_TYPE_S16X4 = BRIG_TYPE_S16 | BRIG_TYPE_PACK_64, //.ctype=int16_t // four short signed integers
    BRIG_TYPE_S16X8 = BRIG_TYPE_S16 | BRIG_TYPE_PACK_128, //.ctype=int16_t // eight short signed integers
    BRIG_TYPE_S32X2 = BRIG_TYPE_S32 | BRIG_TYPE_PACK_64, //.ctype=int32_t // two signed integers
    BRIG_TYPE_S32X4 = BRIG_TYPE_S32 | BRIG_TYPE_PACK_128, //.ctype=int32_t // four signed integers
    BRIG_TYPE_S64X2 = BRIG_TYPE_S64 | BRIG_TYPE_PACK_128, //.ctype=int64_t // two 64-bit signed integers
    BRIG_TYPE_F16X2 = BRIG_TYPE_F16 | BRIG_TYPE_PACK_32, //.ctype=f16_t // two half-floats
    BRIG_TYPE_F16X4 = BRIG_TYPE_F16 | BRIG_TYPE_PACK_64, //.ctype=f16_t // four half-floats
    BRIG_TYPE_F16X8 = BRIG_TYPE_F16 | BRIG_TYPE_PACK_128, //.ctype=f16_t // eight half-floats
    BRIG_TYPE_F32X2 = BRIG_TYPE_F32 | BRIG_TYPE_PACK_64, //.ctype=float // two floats
    BRIG_TYPE_F32X4 = BRIG_TYPE_F32 | BRIG_TYPE_PACK_128, //.ctype=float // four floats
    BRIG_TYPE_F64X2 = BRIG_TYPE_F64 | BRIG_TYPE_PACK_128, //.ctype=double // two doubles

    // Used internally
    BRIG_TYPE_INVALID = -1 //.skip
};

enum BrigVariableModifierMask {
    //.nodump
    BRIG_SYMBOL_DEFINITION = 1,
    BRIG_SYMBOL_CONST = 2,
    BRIG_SYMBOL_ARRAY = 4,
    BRIG_SYMBOL_FLEX_ARRAY = 8
};

enum BrigWidth {
    BRIG_WIDTH_NONE = 0,
    BRIG_WIDTH_1 = 1,
    BRIG_WIDTH_2 = 2,
    BRIG_WIDTH_4 = 3,
    BRIG_WIDTH_8 = 4,
    BRIG_WIDTH_16 = 5,
    BRIG_WIDTH_32 = 6,
    BRIG_WIDTH_64 = 7,
    BRIG_WIDTH_128 = 8,
    BRIG_WIDTH_256 = 9,
    BRIG_WIDTH_512 = 10,
    BRIG_WIDTH_1024 = 11,
    BRIG_WIDTH_2048 = 12,
    BRIG_WIDTH_4096 = 13,
    BRIG_WIDTH_8192 = 14,
    BRIG_WIDTH_16384 = 15,
    BRIG_WIDTH_32768 = 16,
    BRIG_WIDTH_65536 = 17,
    BRIG_WIDTH_131072 = 18,
    BRIG_WIDTH_262144 = 19,
    BRIG_WIDTH_524288 = 20,
    BRIG_WIDTH_1048576 = 21,
    BRIG_WIDTH_2097152 = 22,
    BRIG_WIDTH_4194304 = 23,
    BRIG_WIDTH_8388608 = 24,
    BRIG_WIDTH_16777216 = 25,
    BRIG_WIDTH_33554432 = 26,
    BRIG_WIDTH_67108864 = 27,
    BRIG_WIDTH_134217728 = 28,
    BRIG_WIDTH_268435456 = 29,
    BRIG_WIDTH_536870912 = 30,
    BRIG_WIDTH_1073741824 = 31,
    BRIG_WIDTH_2147483648 = 32,
    BRIG_WIDTH_WAVESIZE = 33,
    BRIG_WIDTH_ALL = 34
};

struct BrigUInt64 { //.isroot //.standalone
    uint32_t lo;     //.defValue=0
    uint32_t hi;     //.defValue=0
    //+hcode KLASS& operator=(uint64_t rhs);
    //+hcode operator uint64_t();
    //+implcode inline KLASS& KLASS::operator=(uint64_t rhs) { lo() = (uint32_t)rhs; hi() = (uint32_t)(rhs >> 32); return *this; }
    //+implcode inline KLASS::operator uint64_t() { return ((uint64_t)hi()) << 32 | lo(); }
};

struct BrigAluModifier { //.isroot //.standalone
    BrigAluModifier16_t allBits; //.defValue=0
    //^^ BrigRound8_t round; //.wtype=BFValRef<Brig::BrigRound8_t,0,5>
    //^^ bool ftz; //.wtype=BitValRef<5>
};

struct BrigBase { //.nowrap
    uint16_t byteCount;
    BrigKinds16_t kind;
};

struct BrigCode { //.generic //.isroot //.section=BRIG_SECTION_INDEX_CODE
    uint16_t byteCount;
    BrigKinds16_t kind;
};

struct BrigData {
    //.nowrap
    uint32_t byteCount;
    uint8_t bytes[1];
};

struct BrigExecutableModifier { //.isroot //.standalone
    BrigExecutableModifier8_t allBits; //.defValue=0
    //^^ bool isDefinition; //.wtype=BitValRef<0>
};

struct BrigMemoryModifier { //.isroot //.standalone
    BrigMemoryModifier8_t allBits; //.defValue=0
    //^^ bool isConst; //.wtype=BitValRef<0>
};

struct BrigSegCvtModifier { //.isroot //.standalone
    BrigSegCvtModifier8_t allBits; //.defValue=0
    //^^ bool isNoNull; //.wtype=BitValRef<0>
};

struct BrigVariableModifier { //.isroot //.standalone
    BrigVariableModifier8_t allBits; //.defValue=0
    //^^ bool isDefinition; //.wtype=BitValRef<0>
    //^^ bool isConst; //.wtype=BitValRef<1>
    //^^ bool isArray; //.wtype=BitValRef<2>
    //^^ bool isFlexArray; //.wtype=BitValRef<3>
};

struct BrigDirective { //.generic //.parent=BrigCode
    uint16_t byteCount;
    BrigKinds16_t kind;
};

struct BrigDirectiveArgBlockEnd {
    uint16_t byteCount;
    BrigKinds16_t kind;
};

struct BrigDirectiveArgBlockStart {
    uint16_t byteCount;
    BrigKinds16_t kind;
};

struct BrigDirectiveComment {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
};

struct BrigDirectiveControl {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigControlDirective16_t control;
    uint16_t reserved; //.defValue=0
    BrigDataOffsetOperandList32_t operands;
};

struct BrigDirectiveExecutable { //.generic
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
    uint16_t outArgCount; //.defValue=0
    uint16_t inArgCount;  //.defValue=0
    BrigCodeOffset32_t firstInArg;
    BrigCodeOffset32_t firstCodeBlockEntry;
    BrigCodeOffset32_t nextModuleEntry;
    uint32_t codeBlockEntryCount;  //.defValue=0
    BrigExecutableModifier modifier; //.acc=subItem<ExecutableModifier> //.wtype=ExecutableModifier
    BrigLinkage8_t linkage;
    uint16_t reserved; //.defValue=0
};

struct BrigDirectiveExtension {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
};

struct BrigDirectiveFbarrier {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
    BrigExecutableModifier modifier; //.acc=subItem<ExecutableModifier> //.wtype=ExecutableModifier
    BrigLinkage8_t linkage;
    uint16_t reserved; //.defValue=0
};

struct BrigDirectiveFunction { //.parent=BrigDirectiveExecutable
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
    uint16_t outArgCount; //.defValue=0
    uint16_t inArgCount;  //.defValue=0
    BrigCodeOffset32_t firstInArg;
    BrigCodeOffset32_t firstCodeBlockEntry;
    BrigCodeOffset32_t nextModuleEntry;
    uint32_t codeBlockEntryCount; //.defValue=0
    BrigExecutableModifier modifier; //.acc=subItem<ExecutableModifier> //.wtype=ExecutableModifier
    BrigLinkage8_t linkage;
    uint16_t reserved; //.defValue=0
};

struct BrigDirectiveIndirectFunction { //.parent=BrigDirectiveExecutable
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
    uint16_t outArgCount; //.defValue=0
    uint16_t inArgCount;  //.defValue=0
    BrigCodeOffset32_t firstInArg;
    BrigCodeOffset32_t firstCodeBlockEntry;
    BrigCodeOffset32_t nextModuleEntry;
    uint32_t codeBlockEntryCount; //.defValue=0
    BrigExecutableModifier modifier; //.acc=subItem<ExecutableModifier> //.wtype=ExecutableModifier
    BrigLinkage8_t linkage;
    uint16_t reserved; //.defValue=0
};

struct BrigDirectiveKernel { //.parent=BrigDirectiveExecutable
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
    uint16_t outArgCount; //.defValue=0
    uint16_t inArgCount;  //.defValue=0
    BrigCodeOffset32_t firstInArg;
    BrigCodeOffset32_t firstCodeBlockEntry;
    BrigCodeOffset32_t nextModuleEntry;
    uint32_t codeBlockEntryCount; //.defValue=0
    BrigExecutableModifier modifier; //.acc=subItem<ExecutableModifier> //.wtype=ExecutableModifier
    BrigLinkage8_t linkage;
    uint16_t reserved; //.defValue=0
};

struct BrigDirectiveSignature { //.parent=BrigDirectiveExecutable
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
    uint16_t outArgCount; //.defValue=0
    uint16_t inArgCount;  //.defValue=0
    BrigCodeOffset32_t firstInArg;
    BrigCodeOffset32_t firstCodeBlockEntry;
    BrigCodeOffset32_t nextModuleEntry;
    uint32_t codeBlockEntryCount; //.defValue=0
    BrigExecutableModifier modifier; //.acc=subItem<ExecutableModifier> //.wtype=ExecutableModifier
    BrigLinkage8_t linkage;
    uint16_t reserved; //.defValue=0
};

struct BrigDirectiveLabel {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
};

struct BrigDirectiveLoc {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t filename;
    uint32_t line;
    uint32_t column; //.defValue=1
};

struct BrigDirectiveNone { //.enum=BRIG_KIND_NONE
    uint16_t byteCount;
    BrigKinds16_t kind;
};

struct BrigDirectivePragma {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetOperandList32_t operands;
};

struct BrigDirectiveVariable {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
    BrigOperandOffset32_t init;
    BrigType16_t type;
    BrigSegment8_t segment;
    BrigAlignment8_t align;
    BrigUInt64 dim; //.acc=subItem<UInt64> //.wtype=UInt64
    BrigVariableModifier modifier; //.acc=subItem<VariableModifier> //.wtype=VariableModifier
    BrigLinkage8_t linkage;
    BrigAllocation8_t allocation;
    uint8_t reserved; //.defValue=0
};

struct BrigDirectiveVersion {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigVersion32_t hsailMajor; //.wtype=ValRef<uint32_t>
    BrigVersion32_t hsailMinor; //.wtype=ValRef<uint32_t>
    BrigVersion32_t brigMajor;  //.wtype=ValRef<uint32_t>
    BrigVersion32_t brigMinor;  //.wtype=ValRef<uint32_t>
    BrigProfile8_t profile;
    BrigMachineModel8_t machineModel;
    uint16_t reserved; //.defValue=0
};

struct BrigInst { //.generic //.parent=BrigCode
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    //+hcode Operand operand(int index);
    //+implcode inline Operand KLASS::operand(int index) { return operands()[index]; }
};

struct BrigInstAddr {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigSegment8_t segment;
    uint8_t reserved[3]; //.defValue=0
};

struct BrigInstAtomic {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigSegment8_t segment;
    BrigMemoryOrder8_t memoryOrder;
    BrigMemoryScope8_t memoryScope;
    BrigAtomicOperation8_t atomicOperation;
    uint8_t equivClass;
    uint8_t reserved[3]; //.defValue=0
};

struct BrigInstBasic {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
};

struct BrigInstBr {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigWidth8_t width;
    uint8_t reserved[3]; //.defValue=0
};

struct BrigInstCmp {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigType16_t sourceType;
    BrigAluModifier modifier; //.acc=subItem<AluModifier> //.wtype=AluModifier
    BrigCompareOperation8_t compare;
    BrigPack8_t pack;
    uint16_t reserved; //.defValue=0
};

struct BrigInstCvt {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigType16_t sourceType;
    BrigAluModifier modifier; //.acc=subItem<AluModifier> //.wtype=AluModifier
};

struct BrigInstImage {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigType16_t imageType;
    BrigType16_t coordType;
    BrigImageGeometry8_t geometry;
    uint8_t equivClass;
    uint16_t reserved; //.defValue=0
};

struct BrigInstLane {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigType16_t sourceType;
    BrigWidth8_t width;
    uint8_t reserved; //.defValue=0
};

struct BrigInstMem {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigSegment8_t segment;
    BrigAlignment8_t align;
    uint8_t equivClass;
    BrigWidth8_t width;
    BrigMemoryModifier modifier; //.acc=subItem<MemoryModifier> //.wtype=MemoryModifier
    uint8_t reserved[3]; //.defValue=0
};

struct BrigInstMemFence {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigMemoryOrder8_t memoryOrder;
    BrigMemoryScope8_t globalSegmentMemoryScope;
    BrigMemoryScope8_t groupSegmentMemoryScope;
    BrigMemoryScope8_t imageSegmentMemoryScope;
};

struct BrigInstMod {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigAluModifier modifier; //.acc=subItem<AluModifier> //.wtype=AluModifier

    BrigPack8_t pack;
    uint8_t reserved; //.defValue=0
};

struct BrigInstQueryImage {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigType16_t imageType;
    BrigImageGeometry8_t geometry;
    BrigImageQuery8_t imageQuery;
};

struct BrigInstQuerySampler {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigSamplerQuery8_t samplerQuery;
    uint8_t reserved[3]; //.defValue=0
};

struct BrigInstQueue {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigSegment8_t segment;
    BrigMemoryOrder8_t memoryOrder;
    uint16_t reserved; //.defValue=0
};

struct BrigInstSeg {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigSegment8_t segment;
    uint8_t reserved[3]; //.defValue=0
};

struct BrigInstSegCvt {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigType16_t sourceType;
    BrigSegment8_t segment;
    BrigSegCvtModifier modifier; //.acc=subItem<SegCvtModifier> //.wtype=SegCvtModifier
};

struct BrigInstSignal {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigType16_t signalType;
    BrigMemoryOrder8_t memoryOrder;
    BrigAtomicOperation8_t signalOperation;
};

struct BrigInstSourceType {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigOpcode16_t opcode;
    BrigType16_t type;
    BrigDataOffsetOperandList32_t operands;
    BrigType16_t sourceType;
    uint16_t reserved; //.defValue=0
};

struct BrigOperand { //.generic //.isroot //.section=BRIG_SECTION_INDEX_OPERAND
    uint16_t byteCount;
    BrigKinds16_t kind;
};


struct BrigOperandAddress {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigCodeOffset32_t symbol; //.wtype=ItemRef<DirectiveVariable>
    BrigOperandOffset32_t reg; //.wtype=ItemRef<OperandReg>
    BrigUInt64 offset; //.acc=subItem<UInt64> //.wtype=UInt64
};

struct BrigOperandCodeList {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetCodeList32_t elements;
    //+hcode unsigned elementCount();
    //+implcode inline unsigned KLASS::elementCount() { return elements().size(); }
    //+hcode Code elements(int index);
    //+implcode inline Code KLASS::elements(int index) { return elements()[index]; }
};

struct BrigOperandCodeRef {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigCodeOffset32_t ref;
};

struct BrigOperandData {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t data;
};

struct BrigOperandImageProperties {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigUInt64 width;  //.acc=subItem<UInt64> //.wtype=UInt64
    BrigUInt64 height; //.acc=subItem<UInt64> //.wtype=UInt64
    BrigUInt64 depth;  //.acc=subItem<UInt64> //.wtype=UInt64
    BrigUInt64 array;  //.acc=subItem<UInt64> //.wtype=UInt64
    BrigImageGeometry8_t geometry;
    BrigImageChannelOrder8_t channelOrder;
    BrigImageChannelType8_t channelType;
    uint8_t reserved; //.defValue=0
};

struct BrigOperandOperandList {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetOperandList32_t elements;
    //+hcode unsigned elementCount();
    //+implcode inline unsigned KLASS::elementCount() { return elements().size(); }
    //+hcode Operand elements(int index);
    //+implcode inline Operand KLASS::elements(int index) { return elements()[index]; }
};

struct BrigOperandReg {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigRegisterKind16_t regKind;
    uint16_t regNum;
};

struct BrigOperandSamplerProperties {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigSamplerCoordNormalization8_t coord;
    BrigSamplerFilter8_t filter;
    BrigSamplerAddressing8_t addressing;
    uint8_t reserved; //.defValue=0
};

struct BrigOperandString {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t string;
};

struct BrigOperandWavesize {
    uint16_t byteCount;
    BrigKinds16_t kind;
};

//.ignore{

struct BrigSectionHeader {
    uint32_t byteCount;
    uint32_t headerByteCount;
    uint32_t nameLength;
    uint8_t name[1];
};

struct BrigModule {
    uint32_t sectionCount;
    BrigSectionHeader* section[1];
};


//}

enum BrigMemoryFenceSegments { // for internal use only
    //.mnemo={ s/^BRIG_MEMORY_FENCE_SEGMENT_//;lc }
    //.mnemo_token=_EMMemoryFenceSegments
    //.mnemo_context=EInstModifierInstFenceContext
    BRIG_MEMORY_FENCE_SEGMENT_GLOBAL = 0,
    BRIG_MEMORY_FENCE_SEGMENT_GROUP = 1,
    BRIG_MEMORY_FENCE_SEGMENT_IMAGE = 2,
    BRIG_MEMORY_FENCE_SEGMENT_LAST = 3 //.skip
};

#endif //HSAILBRIG_H_
