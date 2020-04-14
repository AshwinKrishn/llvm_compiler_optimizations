; ModuleID = 'calctest_m2r_mod.bc'
source_filename = "tests/calctest.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @main() #0 {
  %1 = sext i8 48 to i32
  switch i32 %1, label %16 [
    i32 48, label %2
    i32 43, label %11
    i32 42, label %12
    i32 45, label %13
    i32 47, label %14
    i32 10, label %15
  ]

; <label>:2:                                      ; preds = %0
  %3 = trunc i8 1 to i1
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %2
  br label %10

; <label>:5:                                      ; preds = %2
  %6 = add nsw i32 24, 6
  %7 = icmp ne i32 %6, 0
  br i1 %7, label %8, label %9

; <label>:8:                                      ; preds = %5
  br label %9

; <label>:9:                                      ; preds = %8, %5
  br label %10

; <label>:10:                                     ; preds = %9, %4
  br label %17

; <label>:11:                                     ; preds = %0
  br label %17

; <label>:12:                                     ; preds = %0
  br label %17

; <label>:13:                                     ; preds = %0
  br label %17

; <label>:14:                                     ; preds = %0
  br label %17

; <label>:15:                                     ; preds = %0
  br label %17

; <label>:16:                                     ; preds = %0
  br label %17

; <label>:17:                                     ; preds = %16, %15, %14, %13, %12, %11, %10
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 "}
