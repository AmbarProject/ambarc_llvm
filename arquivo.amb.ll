; ModuleID = 'ambar'
source_filename = "ambar"

@x = internal global i32 10
@y = internal global float 0x40091EB860000000
@z = internal global i1 false
@msg = internal global ptr @.str
@.str = private constant [12 x i8] c"Hello World\00"
@.str.1 = private constant [21 x i8] c"Hello from function!\00"

define void @hello() {
entry:
  store ptr @.str.1, ptr @msg, align 8
  ret void
}

define i32 @sum() {
entry:
  ret i32 1
}

define i32 @main() {
entry:
  %0 = load i32, ptr @x, align 4
  %cmptmp = icmp slt i32 %0, 20
  br i1 %cmptmp, label %then, label %else

then:                                             ; preds = %entry
  %1 = load i32, ptr @x, align 4
  %addtmp = add i32 %1, 1
  store i32 %addtmp, ptr @x, align 4
  br label %ifcont

else:                                             ; preds = %entry
  %2 = load i32, ptr @x, align 4
  %subtmp = sub i32 %2, 1
  store i32 %subtmp, ptr @x, align 4
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  br label %while.cond

while.cond:                                       ; preds = %ifcont5, %then4, %ifcont
  %3 = load i32, ptr @x, align 4
  %cmptmp1 = icmp slt i32 %3, 15
  br i1 %cmptmp1, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %4 = load i32, ptr @x, align 4
  %addtmp2 = add i32 %4, 2
  store i32 %addtmp2, ptr @x, align 4
  %5 = load i32, ptr @x, align 4
  %cmptmp3 = icmp eq i32 %5, 13
  br i1 %cmptmp3, label %then4, label %ifcont5

while.end:                                        ; preds = %while.cond
  ret i32 0

then4:                                            ; preds = %while.body
  br label %while.cond

ifcont5:                                          ; preds = %aftercontinue, %while.body
  br label %while.cond

aftercontinue:                                    ; No predecessors!
  br label %ifcont5
}

define i32 @main.2() {
entry:
  ret i32 0
}
