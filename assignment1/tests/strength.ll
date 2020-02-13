; ModuleID = 'strength.bc'
source_filename = "strength.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @compute(i32 %a, i32 %b) #0 {
entry:
  %mul = mul nsw i32 %a, 2
  %add = add nsw i32 0, %mul
  %mul1 = mul nsw i32 %a, 3
  %add2 = add nsw i32 %add, %mul1
  %mul3 = mul nsw i32 %a, 8
  %add4 = add nsw i32 %add2, %mul3
  %div = sdiv i32 %b, 2
  %sub = sub nsw i32 %add4, %div
  %div5 = sdiv i32 %b, 4
  %sub6 = sub nsw i32 %sub, %div5
  %div7 = sdiv i32 %b, 8
  %sub8 = sub nsw i32 %sub6, %div7
  ret i32 %sub8
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 "}
