; ModuleID = 'calctest_m2r.bc'
source_filename = "calctest.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [21 x i8] c"error: zero divisor\0A\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"\09%.8g\0A\00", align 1
@.str.2 = private unnamed_addr constant [27 x i8] c"error: unknown command %s\0A\00", align 1
@sp = global i32 0, align 4
@val = common global [100 x double] zeroinitializer, align 16
@.str.3 = private unnamed_addr constant [19 x i8] c"error: stack full\0A\00", align 1
@.str.4 = private unnamed_addr constant [20 x i8] c"error: stack empty\0A\00", align 1
@bufp = global i32 0, align 4
@buf = common global [100 x i8] zeroinitializer, align 16
@.str.5 = private unnamed_addr constant [29 x i8] c"ungetch: toomany characters\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define i32 @main() #0 {
  %1 = alloca [100 x i8], align 16
  br label %2

; <label>:2:                                      ; preds = %37, %0
  %3 = getelementptr inbounds [100 x i8], [100 x i8]* %1, i32 0, i32 0
  %4 = call i32 @getop(i8* %3)
  %5 = icmp ne i32 %4, -1
  br i1 %5, label %6, label %38

; <label>:6:                                      ; preds = %2
  switch i32 %4, label %34 [
    i32 48, label %7
    i32 43, label %10
    i32 42, label %14
    i32 45, label %18
    i32 47, label %22
    i32 10, label %31
  ]

; <label>:7:                                      ; preds = %6
  %8 = getelementptr inbounds [100 x i8], [100 x i8]* %1, i32 0, i32 0
  %9 = call double @atof(i8* %8) #4
  call void @push(double %9)
  br label %37

; <label>:10:                                     ; preds = %6
  %11 = call double @pop()
  %12 = call double @pop()
  %13 = fadd double %11, %12
  call void @push(double %13)
  br label %37

; <label>:14:                                     ; preds = %6
  %15 = call double @pop()
  %16 = call double @pop()
  %17 = fmul double %15, %16
  call void @push(double %17)
  br label %37

; <label>:18:                                     ; preds = %6
  %19 = call double @pop()
  %20 = call double @pop()
  %21 = fsub double %20, %19
  call void @push(double %21)
  br label %37

; <label>:22:                                     ; preds = %6
  %23 = call double @pop()
  %24 = fcmp une double %23, 0.000000e+00
  br i1 %24, label %25, label %28

; <label>:25:                                     ; preds = %22
  %26 = call double @pop()
  %27 = fdiv double %26, %23
  call void @push(double %27)
  br label %30

; <label>:28:                                     ; preds = %22
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0))
  br label %30

; <label>:30:                                     ; preds = %28, %25
  br label %37

; <label>:31:                                     ; preds = %6
  %32 = call double @pop()
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), double %32)
  br label %37

; <label>:34:                                     ; preds = %6
  %35 = getelementptr inbounds [100 x i8], [100 x i8]* %1, i32 0, i32 0
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.2, i32 0, i32 0), i8* %35)
  br label %37

; <label>:37:                                     ; preds = %34, %31, %30, %18, %14, %10, %7
  br label %2

; <label>:38:                                     ; preds = %2
  ret i32 0
}

; Function Attrs: noinline nounwind uwtable
define i32 @getop(i8*) #0 {
  br label %2

; <label>:2:                                      ; preds = %12, %1
  %3 = call i32 @getch()
  %4 = trunc i32 %3 to i8
  %5 = getelementptr inbounds i8, i8* %0, i64 0
  store i8 %4, i8* %5, align 1
  %6 = sext i8 %4 to i32
  %7 = icmp eq i32 %6, 32
  br i1 %7, label %10, label %8

; <label>:8:                                      ; preds = %2
  %9 = icmp eq i32 %3, 9
  br label %10

; <label>:10:                                     ; preds = %8, %2
  %11 = phi i1 [ true, %2 ], [ %9, %8 ]
  br i1 %11, label %12, label %13

; <label>:12:                                     ; preds = %10
  br label %2

; <label>:13:                                     ; preds = %10
  %14 = getelementptr inbounds i8, i8* %0, i64 1
  store i8 0, i8* %14, align 1
  %15 = call i16** @__ctype_b_loc() #5
  %16 = load i16*, i16** %15, align 8
  %17 = sext i32 %3 to i64
  %18 = getelementptr inbounds i16, i16* %16, i64 %17
  %19 = load i16, i16* %18, align 2
  %20 = zext i16 %19 to i32
  %21 = and i32 %20, 2048
  %22 = icmp ne i32 %21, 0
  br i1 %22, label %26, label %23

; <label>:23:                                     ; preds = %13
  %24 = icmp ne i32 %3, 46
  br i1 %24, label %25, label %26

; <label>:25:                                     ; preds = %23
  br label %79

; <label>:26:                                     ; preds = %23, %13
  %27 = call i16** @__ctype_b_loc() #5
  %28 = load i16*, i16** %27, align 8
  %29 = sext i32 %3 to i64
  %30 = getelementptr inbounds i16, i16* %28, i64 %29
  %31 = load i16, i16* %30, align 2
  %32 = zext i16 %31 to i32
  %33 = and i32 %32, 2048
  %34 = icmp ne i32 %33, 0
  br i1 %34, label %35, label %53

; <label>:35:                                     ; preds = %26
  br label %36

; <label>:36:                                     ; preds = %51, %35
  %.02 = phi i32 [ 0, %35 ], [ %41, %51 ]
  %37 = call i16** @__ctype_b_loc() #5
  %38 = load i16*, i16** %37, align 8
  %39 = call i32 @getch()
  %40 = trunc i32 %39 to i8
  %41 = add nsw i32 %.02, 1
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds i8, i8* %0, i64 %42
  store i8 %40, i8* %43, align 1
  %44 = sext i8 %40 to i32
  %45 = sext i32 %44 to i64
  %46 = getelementptr inbounds i16, i16* %38, i64 %45
  %47 = load i16, i16* %46, align 2
  %48 = zext i16 %47 to i32
  %49 = and i32 %48, 2048
  %50 = icmp ne i32 %49, 0
  br i1 %50, label %51, label %52

; <label>:51:                                     ; preds = %36
  br label %36

; <label>:52:                                     ; preds = %36
  br label %53

; <label>:53:                                     ; preds = %52, %26
  %.13 = phi i32 [ %41, %52 ], [ 0, %26 ]
  %.01 = phi i32 [ %39, %52 ], [ %3, %26 ]
  %54 = icmp eq i32 %.01, 46
  br i1 %54, label %55, label %73

; <label>:55:                                     ; preds = %53
  br label %56

; <label>:56:                                     ; preds = %71, %55
  %.2 = phi i32 [ %.13, %55 ], [ %61, %71 ]
  %57 = call i16** @__ctype_b_loc() #5
  %58 = load i16*, i16** %57, align 8
  %59 = call i32 @getch()
  %60 = trunc i32 %59 to i8
  %61 = add nsw i32 %.2, 1
  %62 = sext i32 %61 to i64
  %63 = getelementptr inbounds i8, i8* %0, i64 %62
  store i8 %60, i8* %63, align 1
  %64 = sext i8 %60 to i32
  %65 = sext i32 %64 to i64
  %66 = getelementptr inbounds i16, i16* %58, i64 %65
  %67 = load i16, i16* %66, align 2
  %68 = zext i16 %67 to i32
  %69 = and i32 %68, 2048
  %70 = icmp ne i32 %69, 0
  br i1 %70, label %71, label %72

; <label>:71:                                     ; preds = %56
  br label %56

; <label>:72:                                     ; preds = %56
  br label %73

; <label>:73:                                     ; preds = %72, %53
  %.3 = phi i32 [ %61, %72 ], [ %.13, %53 ]
  %.1 = phi i32 [ %59, %72 ], [ %.01, %53 ]
  %74 = sext i32 %.3 to i64
  %75 = getelementptr inbounds i8, i8* %0, i64 %74
  store i8 0, i8* %75, align 1
  %76 = icmp ne i32 %.1, -1
  br i1 %76, label %77, label %78

; <label>:77:                                     ; preds = %73
  call void @ungetch(i32 %.1)
  br label %78

; <label>:78:                                     ; preds = %77, %73
  br label %79

; <label>:79:                                     ; preds = %78, %25
  %.0 = phi i32 [ 48, %78 ], [ %3, %25 ]
  ret i32 %.0
}

; Function Attrs: noinline nounwind uwtable
define void @push(double) #0 {
  %2 = load i32, i32* @sp, align 4
  %3 = icmp slt i32 %2, 100
  br i1 %3, label %4, label %9

; <label>:4:                                      ; preds = %1
  %5 = load i32, i32* @sp, align 4
  %6 = add nsw i32 %5, 1
  store i32 %6, i32* @sp, align 4
  %7 = sext i32 %5 to i64
  %8 = getelementptr inbounds [100 x double], [100 x double]* @val, i64 0, i64 %7
  store double %0, double* %8, align 8
  br label %11

; <label>:9:                                      ; preds = %1
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.3, i32 0, i32 0))
  br label %11

; <label>:11:                                     ; preds = %9, %4
  ret void
}

; Function Attrs: nounwind readonly
declare double @atof(i8*) #1

; Function Attrs: noinline nounwind uwtable
define double @pop() #0 {
  %1 = load i32, i32* @sp, align 4
  %2 = icmp sgt i32 %1, 0
  br i1 %2, label %3, label %9

; <label>:3:                                      ; preds = %0
  %4 = load i32, i32* @sp, align 4
  %5 = add nsw i32 %4, -1
  store i32 %5, i32* @sp, align 4
  %6 = sext i32 %5 to i64
  %7 = getelementptr inbounds [100 x double], [100 x double]* @val, i64 0, i64 %6
  %8 = load double, double* %7, align 8
  br label %11

; <label>:9:                                      ; preds = %0
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.4, i32 0, i32 0))
  br label %11

; <label>:11:                                     ; preds = %9, %3
  %.0 = phi double [ %8, %3 ], [ 0.000000e+00, %9 ]
  ret double %.0
}

declare i32 @printf(i8*, ...) #2

; Function Attrs: noinline nounwind uwtable
define i32 @getch() #0 {
  %1 = load i32, i32* @bufp, align 4
  %2 = icmp sgt i32 %1, 0
  br i1 %2, label %3, label %10

; <label>:3:                                      ; preds = %0
  %4 = load i32, i32* @bufp, align 4
  %5 = add nsw i32 %4, -1
  store i32 %5, i32* @bufp, align 4
  %6 = sext i32 %5 to i64
  %7 = getelementptr inbounds [100 x i8], [100 x i8]* @buf, i64 0, i64 %6
  %8 = load i8, i8* %7, align 1
  %9 = sext i8 %8 to i32
  br label %12

; <label>:10:                                     ; preds = %0
  %11 = call i32 @getchar()
  br label %12

; <label>:12:                                     ; preds = %10, %3
  %13 = phi i32 [ %9, %3 ], [ %11, %10 ]
  ret i32 %13
}

; Function Attrs: nounwind readnone
declare i16** @__ctype_b_loc() #3

; Function Attrs: noinline nounwind uwtable
define void @ungetch(i32) #0 {
  %2 = load i32, i32* @bufp, align 4
  %3 = icmp sge i32 %2, 100
  br i1 %3, label %4, label %6

; <label>:4:                                      ; preds = %1
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.5, i32 0, i32 0))
  br label %12

; <label>:6:                                      ; preds = %1
  %7 = trunc i32 %0 to i8
  %8 = load i32, i32* @bufp, align 4
  %9 = add nsw i32 %8, 1
  store i32 %9, i32* @bufp, align 4
  %10 = sext i32 %8 to i64
  %11 = getelementptr inbounds [100 x i8], [100 x i8]* @buf, i64 0, i64 %10
  store i8 %7, i8* %11, align 1
  br label %12

; <label>:12:                                     ; preds = %6, %4
  ret void
}

declare i32 @getchar() #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readonly }
attributes #5 = { nounwind readnone }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 "}
