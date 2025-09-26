; ModuleID = 'ambar'
source_filename = "ambar"

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 40, ptr %n, align 4
  %a = alloca i32, align 4
  store i32 0, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 1, ptr %b, align 4
  %temp = alloca i32, align 4
  store i32 0, ptr %temp, align 4
  %n.load = load i32, ptr %n, align 4
  %cmptmp = icmp sle i32 %n.load, 1
  br i1 %cmptmp, label %then, label %ifcont

then:                                             ; preds = %entry
  br label %ifcont

ifcont:                                           ; preds = %then, %entry
  ret i32 0
}
