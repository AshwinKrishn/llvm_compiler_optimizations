; ModuleID = 'algebraic.bc'
source_filename = "algebraic.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @compute(i32 %a, i32 %b) #0 {
entry:
  %div = sdiv i32 %a, %a
  %mul = mul nsw i32 %div, 0
  %mul1 = mul nsw i32 %mul, 1
  %div2 = sdiv i32 %mul1, 1
  %div3 = sdiv i32 %b, %b
  %mul4 = mul nsw i32 %div2, %div3
  %sub = sub nsw i32 %b, %b
  %add = add nsw i32 %mul4, %sub
  %div5 = sdiv i32 %add, %add
  %sub6 = sub nsw i32 %div5, %div5
  %add7 = add nsw i32 %sub6, 4
  %mul8 = mul nsw i32 %add7, 2
  %div9 = sdiv i32 %mul8, 1
  ret i32 %div9
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 "}
