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
  ret i32 0
}
