; ModuleID = 'ambar'
source_filename = "ambar"

@fib_result = internal global i32 0

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 1000, ptr %n, align 4
  %a = alloca i32, align 4
  store i32 0, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 1, ptr %b, align 4
  %temp = alloca i32, align 4
  %0 = load i32, ptr %n, align 4
  %cmptmp = icmp eq i32 %0, 0
  br i1 %cmptmp, label %then, label %else

then:                                             ; preds = %entry
  %1 = load i32, ptr %a, align 4
  store i32 %1, ptr @fib_result, align 4
  br label %ifcont

else:                                             ; preds = %entry
  %i = alloca i32, align 4
  store i32 2, ptr %i, align 4
  br label %for.cond

ifcont:                                           ; preds = %for.end, %then
  ret i32 0

for.cond:                                         ; preds = %for.update, %else
  %2 = load i32, ptr %i, align 4
  %3 = load i32, ptr %n, align 4
  %cmptmp1 = icmp sle i32 %2, %3
  br i1 %cmptmp1, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %4 = load i32, ptr %a, align 4
  %5 = load i32, ptr %b, align 4
  %addtmp = add i32 %4, %5
  store i32 %addtmp, ptr %temp, align 4
  %6 = load i32, ptr %b, align 4
  store i32 %6, ptr %a, align 4
  %7 = load i32, ptr %temp, align 4
  store i32 %7, ptr %b, align 4
  br label %for.update

for.update:                                       ; preds = %for.body
  %8 = load i32, ptr %i, align 4
  %addtmp2 = add i32 %8, 1
  store i32 %addtmp2, ptr %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %9 = load i32, ptr %b, align 4
  store i32 %9, ptr @fib_result, align 4
  br label %ifcont
}
