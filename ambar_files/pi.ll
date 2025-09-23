; ModuleID = 'ambar'
source_filename = "ambar"

define i32 @main() {
entry:
  %iterations = alloca i32, align 4
  store i32 100000000, ptr %iterations, align 4
  %pi = alloca float, align 4
  store float 0.000000e+00, ptr %pi, align 4
  %sign = alloca float, align 4
  store float 1.000000e+00, ptr %sign, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.update, %entry
  %i.load = load i32, ptr %i, align 4
  %iterations.load = load i32, ptr %iterations, align 4
  %cmptmp = icmp slt i32 %i.load, %iterations.load
  br i1 %cmptmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %pi.load = load float, ptr %pi, align 4
  %sign.load = load float, ptr %sign, align 4
  %i.load1 = load i32, ptr %i, align 4
  %sitofptmp = sitofp i32 %i.load1 to float
  %multmp = fmul float 2.000000e+00, %sitofptmp
  %addtmp = fadd float %multmp, 1.000000e+00
  %divtmp = fdiv float %sign.load, %addtmp
  %addtmp2 = fadd float %pi.load, %divtmp
  store float %addtmp2, ptr %pi, align 4
  %sign.load3 = load float, ptr %sign, align 4
  %negtmp = fneg float %sign.load3
  store float %negtmp, ptr %sign, align 4
  br label %for.update

for.update:                                       ; preds = %for.body
  %i.load4 = load i32, ptr %i, align 4
  %addtmp5 = add i32 %i.load4, 1
  store i32 %addtmp5, ptr %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret i32 0
}
