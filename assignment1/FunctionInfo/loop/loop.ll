; ModuleID = 'loop.bc'
source_filename = "loop.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@g = common local_unnamed_addr global i32 0, align 4

; Function Attrs: norecurse nounwind uwtable
define i32 @change_g(i32 %l) local_unnamed_addr #0 {
entry:
  %0 = load i32, i32* @g, align 4, !tbaa !2
  %add = add nsw i32 %0, %l
  store i32 %add, i32* @g, align 4, !tbaa !2
  ret i32 %add
}

; Function Attrs: norecurse nounwind uwtable
define i32 @sample(i32 %a, i32 %b) local_unnamed_addr #0 {
entry:
  %cmp3 = icmp sgt i32 %a, 0
  %0 = load i32, i32* @g, align 4, !tbaa !2
  br i1 %cmp3, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %entry
  %1 = mul i32 %b, %a
  %2 = add i32 %0, %1
  store i32 %2, i32* @g, align 4, !tbaa !2
  br label %for.end

for.end:                                          ; preds = %for.body.lr.ph, %entry
  %.lcssa = phi i32 [ %2, %for.body.lr.ph ], [ %0, %entry ]
  ret i32 %.lcssa
}

attributes #0 = { norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
