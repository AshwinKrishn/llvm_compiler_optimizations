; ModuleID = 'calctest_m2r.bc'
source_filename = "calctest.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@global_var = global i32 0, align 4

; Function Attrs: noinline nounwind uwtable
define i32 @main() #0 {
  br label %1

; <label>:1:                                      ; preds = %10, %0
  %.0 = phi i32 [ undef, %0 ], [ %.1, %10 ]
  %2 = call i32 @getop(i8 signext undef)
  %3 = icmp ne i32 %2, -1
  br i1 %3, label %4, label %11

; <label>:4:                                      ; preds = %1
  %5 = fcmp ogt double undef, 1.000000e+00
  br i1 %5, label %6, label %8

; <label>:6:                                      ; preds = %4
  %7 = add nsw i32 undef, %.0
  br label %10

; <label>:8:                                      ; preds = %4
  %9 = add nsw i32 undef, undef
  br label %10

; <label>:10:                                     ; preds = %8, %6
  %.1 = phi i32 [ %.0, %6 ], [ %9, %8 ]
  br label %1

; <label>:11:                                     ; preds = %1
  ret i32 0
}

declare i32 @getop(i8 signext) #1

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 "}
