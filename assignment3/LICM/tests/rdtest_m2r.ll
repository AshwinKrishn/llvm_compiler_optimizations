; ModuleID = 'test_m2r.bc'
source_filename = "rdtest.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @main() #0 {
  %1 = trunc i8 1 to i1
  br i1 %1, label %2, label %3

; <label>:2:                                      ; preds = %0
  br label %4

; <label>:3:                                      ; preds = %0
  br label %4

; <label>:4:                                      ; preds = %3, %2
  %.01 = phi i32 [ 1, %2 ], [ 0, %3 ]
  %.0 = phi i32 [ 2, %2 ], [ 1, %3 ]
  %5 = sub nsw i32 %.01, %.0
  %6 = trunc i8 0 to i1
  br i1 %6, label %7, label %9

; <label>:7:                                      ; preds = %4
  %8 = add nsw i32 %5, 1
  br label %9

; <label>:9:                                      ; preds = %7, %4
  %.02 = phi i32 [ %8, %7 ], [ %5, %4 ]
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 "}
