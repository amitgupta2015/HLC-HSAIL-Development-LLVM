; RUN: llc -march=hsail -verify-machineinstrs < %s | FileCheck -check-prefix=HSAIL -check-prefix=FUNC %s

; HSAIL-DAG: readonly_f32 &float_gv[5] = {0F00000000, 0F3f800000, 0F40000000, 0F40400000, 0F40800000};
; HSAIL-DAG: readonly_u32 &i32_gv[5] = {0, 1, 2, 3, 4};
; HSAIL-DAG: align(8) readonly_u8 &struct_foo_gv[24] = {0, 0, 128, 65, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0};
; HSAIL-DAG: readonly_u32 &array_v1_gv[4] = {1, 2, 3, 4};

@b = internal addrspace(2) constant [1 x i16] [ i16 7 ], align 2

@float_gv = internal unnamed_addr addrspace(2) constant [5 x float] [float 0.0, float 1.0, float 2.0, float 3.0, float 4.0], align 4

%struct.foo = type { float, [5 x i32] }

@struct_foo_gv = internal unnamed_addr addrspace(2) constant [1 x %struct.foo] [ %struct.foo { float 16.0, [5 x i32] [i32 0, i32 1, i32 2, i32 3, i32 4] } ]

@array_v1_gv = internal addrspace(2) constant [4 x <1 x i32>] [ <1 x i32> <i32 1>,
                                                                <1 x i32> <i32 2>,
                                                                <1 x i32> <i32 3>,
                                                                <1 x i32> <i32 4> ]

; FUNC-LABEL: @float
; HSAIL: shl_u32 [[ADDR:\$s[0-9]+]], {{\$s[0-9]+}}, 2;
; HSAIL: ld_readonly_f32 [[LD:\$s[0-9]+]], [&float_gv]{{\[}}[[ADDR]]{{\]}};
; HSAIL: st_global_f32 [[LD]]
; HSAIL: ret;
define void @float(float addrspace(1)* %out, i32 %index) {
  %tmp0 = getelementptr inbounds [5 x float] addrspace(2)* @float_gv, i32 0, i32 %index
  %tmp1 = load float addrspace(2)* %tmp0
  store float %tmp1, float addrspace(1)* %out
  ret void
}

@i32_gv = internal unnamed_addr addrspace(2) constant [5 x i32] [i32 0, i32 1, i32 2, i32 3, i32 4], align 4

; FUNC-LABEL: @i32
; HSAIL: shl_u32 [[ADDR:\$s[0-9]+]], {{\$s[0-9]+}}, 2;
; HSAIL: ld_readonly_u32 [[LD:\$s[0-9]+]], [&i32_gv]{{\[}}[[ADDR]]{{\]}};
; HSAIL: st_global_u32 [[LD]]
; HSAIL: ret;
define void @i32(i32 addrspace(1)* %out, i32 %index) {
  %tmp0 = getelementptr inbounds [5 x i32] addrspace(2)* @i32_gv, i32 0, i32 %index
  %tmp1 = load i32 addrspace(2)* %tmp0
  store i32 %tmp1, i32 addrspace(1)* %out
  ret void
}

; FUNC-LABEL: @struct_foo_gv_load
; HSAIL: shl_u32 [[ADDR:\$s[0-9]+]], {{\$s[0-9]+}}, 2;
; HSAIL: ld_readonly_u32 [[LD:\$s[0-9]+]], [&struct_foo_gv]{{\[}}[[ADDR]]+4{{\]}};
; HSAIL: st_global_u32 [[LD]]
; HSAIL: ret;
define void @struct_foo_gv_load(i32 addrspace(1)* %out, i32 %index) {
  %gep = getelementptr inbounds [1 x %struct.foo] addrspace(2)* @struct_foo_gv, i32 0, i32 0, i32 1, i32 %index
  %load = load i32 addrspace(2)* %gep, align 4
  store i32 %load, i32 addrspace(1)* %out, align 4
  ret void
}


; FUNC-LABEL: @array_v1_gv_load
; HSAIL: shl_u32 [[ADDR:\$s[0-9]+]], {{\$s[0-9]+}}, 2;
; HSAIL: ld_readonly_u32 [[LD:\$s[0-9]+]], [&array_v1_gv]{{\[}}[[ADDR]]{{\]}};
; HSAIL: st_global_u32 [[LD]]
; HSAIL: ret;
define void @array_v1_gv_load(<1 x i32> addrspace(1)* %out, i32 %index) {
  %gep = getelementptr inbounds [4 x <1 x i32>] addrspace(2)* @array_v1_gv, i32 0, i32 %index
  %load = load <1 x i32> addrspace(2)* %gep, align 4
  store <1 x i32> %load, <1 x i32> addrspace(1)* %out, align 4
  ret void
}

; FUNC-LABEL: @gv_addressing_in_branch
; HSAIL: shl_u32 [[ADDR:\$s[0-9]+]], {{\$s[0-9]+}}, 2;
; HSAIL: ld_readonly_f32 [[LD:\$s[0-9]+]], [&float_gv]{{\[}}[[ADDR]]{{\]}};
; HSAIL: st_global_f32 [[LD]]
; HSAIL: ret;
define void @gv_addressing_in_branch(float addrspace(1)* %out, i32 %index, i32 %a) {
entry:
  %tmp0 = icmp eq i32 0, %a
  br i1 %tmp0, label %if, label %else

if:
  %tmp1 = getelementptr inbounds [5 x float] addrspace(2)* @float_gv, i32 0, i32 %index
  %tmp2 = load float addrspace(2)* %tmp1
  store float %tmp2, float addrspace(1)* %out
  br label %endif

else:
  store float 1.0, float addrspace(1)* %out
  br label %endif

endif:
  ret void
}
