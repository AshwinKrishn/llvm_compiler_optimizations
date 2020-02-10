; ModuleID = 'ms.bc'
source_filename = "ms.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.__va_list_tag = type { i32, i32, i8*, i8* }

@.str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@main.arr = private unnamed_addr constant [6 x i32] [i32 12, i32 11, i32 13, i32 5, i32 6, i32 7], align 16
@str = private unnamed_addr constant [16 x i8] c"Given array is \00"
@str.4 = private unnamed_addr constant [18 x i8] c"\0ASorted array is \00"

; Function Attrs: nounwind uwtable
define i32 @variable(i32 %num, ...) local_unnamed_addr #0 {
entry:
  %ap = alloca [1 x %struct.__va_list_tag], align 16
  %0 = bitcast [1 x %struct.__va_list_tag]* %ap to i8*
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %0) #2
  call void @llvm.va_start(i8* nonnull %0)
  %cmp7 = icmp sgt i32 %num, 0
  br i1 %cmp7, label %while.body.lr.ph, label %while.end

while.body.lr.ph:                                 ; preds = %entry
  %gp_offset_p = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %ap, i64 0, i64 0, i32 0
  %1 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %ap, i64 0, i64 0, i32 3
  %overflow_arg_area_p = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %ap, i64 0, i64 0, i32 2
  %gp_offset.pre = load i32, i32* %gp_offset_p, align 16
  %xtraiter = and i32 %num, 1
  %2 = icmp eq i32 %num, 1
  br i1 %2, label %while.end.loopexit.unr-lcssa, label %while.body.lr.ph.new

while.body.lr.ph.new:                             ; preds = %while.body.lr.ph
  %unroll_iter = sub i32 %num, %xtraiter
  br label %while.body

while.body:                                       ; preds = %vaarg.end.1, %while.body.lr.ph.new
  %gp_offset = phi i32 [ %gp_offset.pre, %while.body.lr.ph.new ], [ %gp_offset10.1, %vaarg.end.1 ]
  %a.08 = phi i32 [ 500, %while.body.lr.ph.new ], [ %mul.1, %vaarg.end.1 ]
  %niter = phi i32 [ %unroll_iter, %while.body.lr.ph.new ], [ %niter.nsub.1, %vaarg.end.1 ]
  %fits_in_gp = icmp ult i32 %gp_offset, 41
  br i1 %fits_in_gp, label %vaarg.in_reg, label %vaarg.in_mem

vaarg.in_reg:                                     ; preds = %while.body
  %reg_save_area = load i8*, i8** %1, align 16
  %3 = sext i32 %gp_offset to i64
  %4 = getelementptr i8, i8* %reg_save_area, i64 %3
  %5 = add i32 %gp_offset, 8
  store i32 %5, i32* %gp_offset_p, align 16
  br label %vaarg.end

vaarg.in_mem:                                     ; preds = %while.body
  %overflow_arg_area = load i8*, i8** %overflow_arg_area_p, align 8
  %overflow_arg_area.next = getelementptr i8, i8* %overflow_arg_area, i64 8
  store i8* %overflow_arg_area.next, i8** %overflow_arg_area_p, align 8
  br label %vaarg.end

vaarg.end:                                        ; preds = %vaarg.in_mem, %vaarg.in_reg
  %gp_offset10 = phi i32 [ %5, %vaarg.in_reg ], [ %gp_offset, %vaarg.in_mem ]
  %vaarg.addr.in = phi i8* [ %4, %vaarg.in_reg ], [ %overflow_arg_area, %vaarg.in_mem ]
  %vaarg.addr = bitcast i8* %vaarg.addr.in to i32*
  %6 = load i32, i32* %vaarg.addr, align 4
  %mul = mul nsw i32 %6, %a.08
  %fits_in_gp.1 = icmp ult i32 %gp_offset10, 41
  br i1 %fits_in_gp.1, label %vaarg.in_reg.1, label %vaarg.in_mem.1

while.end.loopexit.unr-lcssa:                     ; preds = %vaarg.end.1, %while.body.lr.ph
  %mul.lcssa.ph = phi i32 [ undef, %while.body.lr.ph ], [ %mul.1, %vaarg.end.1 ]
  %gp_offset.unr = phi i32 [ %gp_offset.pre, %while.body.lr.ph ], [ %gp_offset10.1, %vaarg.end.1 ]
  %a.08.unr = phi i32 [ 500, %while.body.lr.ph ], [ %mul.1, %vaarg.end.1 ]
  %lcmp.mod = icmp eq i32 %xtraiter, 0
  br i1 %lcmp.mod, label %while.end, label %while.body.epil

while.body.epil:                                  ; preds = %while.end.loopexit.unr-lcssa
  %fits_in_gp.epil = icmp ult i32 %gp_offset.unr, 41
  br i1 %fits_in_gp.epil, label %vaarg.in_reg.epil, label %vaarg.in_mem.epil

vaarg.in_mem.epil:                                ; preds = %while.body.epil
  %overflow_arg_area.epil = load i8*, i8** %overflow_arg_area_p, align 8
  %overflow_arg_area.next.epil = getelementptr i8, i8* %overflow_arg_area.epil, i64 8
  store i8* %overflow_arg_area.next.epil, i8** %overflow_arg_area_p, align 8
  br label %while.end.loopexit.epilog-lcssa

vaarg.in_reg.epil:                                ; preds = %while.body.epil
  %reg_save_area.epil = load i8*, i8** %1, align 16
  %7 = sext i32 %gp_offset.unr to i64
  %8 = getelementptr i8, i8* %reg_save_area.epil, i64 %7
  %9 = add i32 %gp_offset.unr, 8
  store i32 %9, i32* %gp_offset_p, align 16
  br label %while.end.loopexit.epilog-lcssa

while.end.loopexit.epilog-lcssa:                  ; preds = %vaarg.in_mem.epil, %vaarg.in_reg.epil
  %vaarg.addr.in.epil = phi i8* [ %8, %vaarg.in_reg.epil ], [ %overflow_arg_area.epil, %vaarg.in_mem.epil ]
  %vaarg.addr.epil = bitcast i8* %vaarg.addr.in.epil to i32*
  %10 = load i32, i32* %vaarg.addr.epil, align 4
  %mul.epil = mul nsw i32 %10, %a.08.unr
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit.epilog-lcssa, %while.end.loopexit.unr-lcssa, %entry
  %a.0.lcssa = phi i32 [ 500, %entry ], [ %mul.lcssa.ph, %while.end.loopexit.unr-lcssa ], [ %mul.epil, %while.end.loopexit.epilog-lcssa ]
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %0) #2
  ret i32 %a.0.lcssa

vaarg.in_mem.1:                                   ; preds = %vaarg.end
  %overflow_arg_area.1 = load i8*, i8** %overflow_arg_area_p, align 8
  %overflow_arg_area.next.1 = getelementptr i8, i8* %overflow_arg_area.1, i64 8
  store i8* %overflow_arg_area.next.1, i8** %overflow_arg_area_p, align 8
  br label %vaarg.end.1

vaarg.in_reg.1:                                   ; preds = %vaarg.end
  %reg_save_area.1 = load i8*, i8** %1, align 16
  %11 = sext i32 %gp_offset10 to i64
  %12 = getelementptr i8, i8* %reg_save_area.1, i64 %11
  %13 = add i32 %gp_offset10, 8
  store i32 %13, i32* %gp_offset_p, align 16
  br label %vaarg.end.1

vaarg.end.1:                                      ; preds = %vaarg.in_reg.1, %vaarg.in_mem.1
  %gp_offset10.1 = phi i32 [ %13, %vaarg.in_reg.1 ], [ %gp_offset10, %vaarg.in_mem.1 ]
  %vaarg.addr.in.1 = phi i8* [ %12, %vaarg.in_reg.1 ], [ %overflow_arg_area.1, %vaarg.in_mem.1 ]
  %vaarg.addr.1 = bitcast i8* %vaarg.addr.in.1 to i32*
  %14 = load i32, i32* %vaarg.addr.1, align 4
  %mul.1 = mul nsw i32 %14, %mul
  %niter.nsub.1 = add i32 %niter, -2
  %niter.ncmp.1 = icmp eq i32 %niter.nsub.1, 0
  br i1 %niter.ncmp.1, label %while.end.loopexit.unr-lcssa, label %while.body
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: norecurse nounwind uwtable
define void @merge(i32* nocapture %arr, i32 %l, i32 %m, i32 %r) local_unnamed_addr #3 {
entry:
  %sub = sub nsw i32 %m, %l
  %add = add nsw i32 %sub, 1
  %sub1 = sub nsw i32 %r, %m
  %0 = zext i32 %add to i64
  %vla = alloca i32, i64 %0, align 16
  %1 = zext i32 %sub1 to i64
  %vla2 = alloca i32, i64 %1, align 16
  %cmp119 = icmp slt i32 %sub, 0
  br i1 %cmp119, label %for.end, label %for.body.lr.ph

for.body.lr.ph:                                   ; preds = %entry
  %vla141 = bitcast i32* %vla to i8*
  %2 = sext i32 %l to i64
  %3 = add i32 %m, 1
  %4 = sub i32 %3, %l
  %scevgep142 = getelementptr i32, i32* %arr, i64 %2
  %scevgep142143 = bitcast i32* %scevgep142 to i8*
  %5 = zext i32 %4 to i64
  %6 = shl nuw nsw i64 %5, 2
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull %vla141, i8* %scevgep142143, i64 %6, i32 4, i1 false)
  br label %for.end

for.end:                                          ; preds = %for.body.lr.ph, %entry
  %cmp7117 = icmp sgt i32 %sub1, 0
  br i1 %cmp7117, label %for.body8.lr.ph, label %while.end

for.body8.lr.ph:                                  ; preds = %for.end
  %min.iters.check = icmp ult i32 %sub1, 8
  br i1 %min.iters.check, label %for.body8.preheader, label %vector.scevcheck

vector.scevcheck:                                 ; preds = %for.body8.lr.ph
  %7 = add nsw i64 %1, -1
  %8 = add i32 %m, 1
  %9 = trunc i64 %7 to i32
  %10 = add i32 %8, %9
  %11 = icmp slt i32 %10, %8
  %12 = icmp ugt i64 %7, 4294967295
  %13 = or i1 %11, %12
  br i1 %13, label %for.body8.preheader, label %vector.ph

vector.ph:                                        ; preds = %vector.scevcheck
  %n.vec = and i64 %1, 4294967288
  %14 = add nsw i64 %n.vec, -8
  %15 = lshr exact i64 %14, 3
  %16 = add nuw nsw i64 %15, 1
  %xtraiter171 = and i64 %16, 1
  %17 = icmp eq i64 %14, 0
  br i1 %17, label %middle.block.unr-lcssa, label %vector.ph.new

vector.ph.new:                                    ; preds = %vector.ph
  %unroll_iter173 = sub nsw i64 %16, %xtraiter171
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph.new
  %index = phi i64 [ 0, %vector.ph.new ], [ %index.next.1, %vector.body ]
  %niter174 = phi i64 [ %unroll_iter173, %vector.ph.new ], [ %niter174.nsub.1, %vector.body ]
  %18 = trunc i64 %index to i32
  %19 = or i32 %18, 1
  %20 = add i32 %19, %m
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds i32, i32* %arr, i64 %21
  %23 = bitcast i32* %22 to <4 x i32>*
  %wide.load = load <4 x i32>, <4 x i32>* %23, align 4, !tbaa !2
  %24 = getelementptr i32, i32* %22, i64 4
  %25 = bitcast i32* %24 to <4 x i32>*
  %wide.load147 = load <4 x i32>, <4 x i32>* %25, align 4, !tbaa !2
  %26 = getelementptr inbounds i32, i32* %vla2, i64 %index
  %27 = bitcast i32* %26 to <4 x i32>*
  store <4 x i32> %wide.load, <4 x i32>* %27, align 16, !tbaa !2
  %28 = getelementptr i32, i32* %26, i64 4
  %29 = bitcast i32* %28 to <4 x i32>*
  store <4 x i32> %wide.load147, <4 x i32>* %29, align 16, !tbaa !2
  %index.next = or i64 %index, 8
  %30 = trunc i64 %index.next to i32
  %31 = or i32 %30, 1
  %32 = add i32 %31, %m
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds i32, i32* %arr, i64 %33
  %35 = bitcast i32* %34 to <4 x i32>*
  %wide.load.1 = load <4 x i32>, <4 x i32>* %35, align 4, !tbaa !2
  %36 = getelementptr i32, i32* %34, i64 4
  %37 = bitcast i32* %36 to <4 x i32>*
  %wide.load147.1 = load <4 x i32>, <4 x i32>* %37, align 4, !tbaa !2
  %38 = getelementptr inbounds i32, i32* %vla2, i64 %index.next
  %39 = bitcast i32* %38 to <4 x i32>*
  store <4 x i32> %wide.load.1, <4 x i32>* %39, align 16, !tbaa !2
  %40 = getelementptr i32, i32* %38, i64 4
  %41 = bitcast i32* %40 to <4 x i32>*
  store <4 x i32> %wide.load147.1, <4 x i32>* %41, align 16, !tbaa !2
  %index.next.1 = add i64 %index, 16
  %niter174.nsub.1 = add i64 %niter174, -2
  %niter174.ncmp.1 = icmp eq i64 %niter174.nsub.1, 0
  br i1 %niter174.ncmp.1, label %middle.block.unr-lcssa, label %vector.body, !llvm.loop !6

middle.block.unr-lcssa:                           ; preds = %vector.body, %vector.ph
  %index.unr = phi i64 [ 0, %vector.ph ], [ %index.next.1, %vector.body ]
  %lcmp.mod172 = icmp eq i64 %xtraiter171, 0
  br i1 %lcmp.mod172, label %middle.block, label %vector.body.epil

vector.body.epil:                                 ; preds = %middle.block.unr-lcssa
  %42 = trunc i64 %index.unr to i32
  %43 = or i32 %42, 1
  %44 = add i32 %43, %m
  %45 = sext i32 %44 to i64
  %46 = getelementptr inbounds i32, i32* %arr, i64 %45
  %47 = bitcast i32* %46 to <4 x i32>*
  %wide.load.epil = load <4 x i32>, <4 x i32>* %47, align 4, !tbaa !2
  %48 = getelementptr i32, i32* %46, i64 4
  %49 = bitcast i32* %48 to <4 x i32>*
  %wide.load147.epil = load <4 x i32>, <4 x i32>* %49, align 4, !tbaa !2
  %50 = getelementptr inbounds i32, i32* %vla2, i64 %index.unr
  %51 = bitcast i32* %50 to <4 x i32>*
  store <4 x i32> %wide.load.epil, <4 x i32>* %51, align 16, !tbaa !2
  %52 = getelementptr i32, i32* %50, i64 4
  %53 = bitcast i32* %52 to <4 x i32>*
  store <4 x i32> %wide.load147.epil, <4 x i32>* %53, align 16, !tbaa !2
  br label %middle.block

middle.block:                                     ; preds = %middle.block.unr-lcssa, %vector.body.epil
  %cmp.n = icmp eq i64 %n.vec, %1
  br i1 %cmp.n, label %for.end17, label %for.body8.preheader

for.body8.preheader:                              ; preds = %middle.block, %vector.scevcheck, %for.body8.lr.ph
  %indvars.iv134.ph = phi i64 [ 0, %vector.scevcheck ], [ 0, %for.body8.lr.ph ], [ %n.vec, %middle.block ]
  %54 = add nsw i64 %1, -1
  %55 = sub nsw i64 %54, %indvars.iv134.ph
  %xtraiter169 = and i64 %1, 3
  %lcmp.mod170 = icmp eq i64 %xtraiter169, 0
  br i1 %lcmp.mod170, label %for.body8.prol.loopexit, label %for.body8.prol.preheader

for.body8.prol.preheader:                         ; preds = %for.body8.preheader
  br label %for.body8.prol

for.body8.prol:                                   ; preds = %for.body8.prol, %for.body8.prol.preheader
  %indvars.iv134.prol = phi i64 [ %indvars.iv.next135.prol, %for.body8.prol ], [ %indvars.iv134.ph, %for.body8.prol.preheader ]
  %prol.iter = phi i64 [ %prol.iter.sub, %for.body8.prol ], [ %xtraiter169, %for.body8.prol.preheader ]
  %indvars.iv.next135.prol = add nuw nsw i64 %indvars.iv134.prol, 1
  %56 = trunc i64 %indvars.iv.next135.prol to i32
  %add10.prol = add i32 %56, %m
  %idxprom11.prol = sext i32 %add10.prol to i64
  %arrayidx12.prol = getelementptr inbounds i32, i32* %arr, i64 %idxprom11.prol
  %57 = load i32, i32* %arrayidx12.prol, align 4, !tbaa !2
  %arrayidx14.prol = getelementptr inbounds i32, i32* %vla2, i64 %indvars.iv134.prol
  store i32 %57, i32* %arrayidx14.prol, align 4, !tbaa !2
  %prol.iter.sub = add i64 %prol.iter, -1
  %prol.iter.cmp = icmp eq i64 %prol.iter.sub, 0
  br i1 %prol.iter.cmp, label %for.body8.prol.loopexit, label %for.body8.prol, !llvm.loop !8

for.body8.prol.loopexit:                          ; preds = %for.body8.prol, %for.body8.preheader
  %indvars.iv134.unr = phi i64 [ %indvars.iv134.ph, %for.body8.preheader ], [ %indvars.iv.next135.prol, %for.body8.prol ]
  %58 = icmp ult i64 %55, 3
  br i1 %58, label %for.end17, label %for.body8.preheader.new

for.body8.preheader.new:                          ; preds = %for.body8.prol.loopexit
  br label %for.body8

for.body8:                                        ; preds = %for.body8, %for.body8.preheader.new
  %indvars.iv134 = phi i64 [ %indvars.iv134.unr, %for.body8.preheader.new ], [ %indvars.iv.next135.3, %for.body8 ]
  %indvars.iv.next135 = add nuw nsw i64 %indvars.iv134, 1
  %59 = trunc i64 %indvars.iv.next135 to i32
  %add10 = add i32 %59, %m
  %idxprom11 = sext i32 %add10 to i64
  %arrayidx12 = getelementptr inbounds i32, i32* %arr, i64 %idxprom11
  %60 = load i32, i32* %arrayidx12, align 4, !tbaa !2
  %arrayidx14 = getelementptr inbounds i32, i32* %vla2, i64 %indvars.iv134
  store i32 %60, i32* %arrayidx14, align 4, !tbaa !2
  %indvars.iv.next135.1 = add nsw i64 %indvars.iv134, 2
  %61 = trunc i64 %indvars.iv.next135.1 to i32
  %add10.1 = add i32 %61, %m
  %idxprom11.1 = sext i32 %add10.1 to i64
  %arrayidx12.1 = getelementptr inbounds i32, i32* %arr, i64 %idxprom11.1
  %62 = load i32, i32* %arrayidx12.1, align 4, !tbaa !2
  %arrayidx14.1 = getelementptr inbounds i32, i32* %vla2, i64 %indvars.iv.next135
  store i32 %62, i32* %arrayidx14.1, align 4, !tbaa !2
  %indvars.iv.next135.2 = add nsw i64 %indvars.iv134, 3
  %63 = trunc i64 %indvars.iv.next135.2 to i32
  %add10.2 = add i32 %63, %m
  %idxprom11.2 = sext i32 %add10.2 to i64
  %arrayidx12.2 = getelementptr inbounds i32, i32* %arr, i64 %idxprom11.2
  %64 = load i32, i32* %arrayidx12.2, align 4, !tbaa !2
  %arrayidx14.2 = getelementptr inbounds i32, i32* %vla2, i64 %indvars.iv.next135.1
  store i32 %64, i32* %arrayidx14.2, align 4, !tbaa !2
  %indvars.iv.next135.3 = add nsw i64 %indvars.iv134, 4
  %65 = trunc i64 %indvars.iv.next135.3 to i32
  %add10.3 = add i32 %65, %m
  %idxprom11.3 = sext i32 %add10.3 to i64
  %arrayidx12.3 = getelementptr inbounds i32, i32* %arr, i64 %idxprom11.3
  %66 = load i32, i32* %arrayidx12.3, align 4, !tbaa !2
  %arrayidx14.3 = getelementptr inbounds i32, i32* %vla2, i64 %indvars.iv.next135.2
  store i32 %66, i32* %arrayidx14.3, align 4, !tbaa !2
  %exitcond.3 = icmp eq i64 %indvars.iv.next135.3, %1
  br i1 %exitcond.3, label %for.end17, label %for.body8, !llvm.loop !10

for.end17:                                        ; preds = %for.body8.prol.loopexit, %for.body8, %middle.block
  %cmp18108 = icmp sgt i32 %sub, -1
  %or.cond110 = and i1 %cmp7117, %cmp18108
  br i1 %or.cond110, label %while.body.lr.ph, label %while.end

while.body.lr.ph:                                 ; preds = %for.end17
  %67 = sext i32 %l to i64
  br label %while.body

while.body:                                       ; preds = %while.body.lr.ph, %if.end
  %indvars.iv132 = phi i64 [ %67, %while.body.lr.ph ], [ %indvars.iv.next133, %if.end ]
  %i.1113 = phi i32 [ 0, %while.body.lr.ph ], [ %i.2, %if.end ]
  %j.1112 = phi i32 [ 0, %while.body.lr.ph ], [ %j.2, %if.end ]
  %idxprom20 = sext i32 %i.1113 to i64
  %arrayidx21 = getelementptr inbounds i32, i32* %vla, i64 %idxprom20
  %68 = load i32, i32* %arrayidx21, align 4, !tbaa !2
  %idxprom22 = sext i32 %j.1112 to i64
  %arrayidx23 = getelementptr inbounds i32, i32* %vla2, i64 %idxprom22
  %69 = load i32, i32* %arrayidx23, align 4, !tbaa !2
  %cmp24 = icmp sgt i32 %68, %69
  %arrayidx33 = getelementptr inbounds i32, i32* %arr, i64 %indvars.iv132
  br i1 %cmp24, label %if.else, label %if.then

if.then:                                          ; preds = %while.body
  store i32 %68, i32* %arrayidx33, align 4, !tbaa !2
  %inc29 = add nsw i32 %i.1113, 1
  br label %if.end

if.else:                                          ; preds = %while.body
  store i32 %69, i32* %arrayidx33, align 4, !tbaa !2
  %inc34 = add nsw i32 %j.1112, 1
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %j.2 = phi i32 [ %j.1112, %if.then ], [ %inc34, %if.else ]
  %i.2 = phi i32 [ %inc29, %if.then ], [ %i.1113, %if.else ]
  %indvars.iv.next133 = add i64 %indvars.iv132, 1
  %cmp18 = icmp sle i32 %i.2, %sub
  %cmp19 = icmp slt i32 %j.2, %sub1
  %or.cond = and i1 %cmp19, %cmp18
  br i1 %or.cond, label %while.body, label %while.end.loopexit

while.end.loopexit:                               ; preds = %if.end
  %70 = trunc i64 %indvars.iv.next133 to i32
  br label %while.end

while.end:                                        ; preds = %for.end, %while.end.loopexit, %for.end17
  %k.0.lcssa = phi i32 [ %l, %for.end17 ], [ %70, %while.end.loopexit ], [ %l, %for.end ]
  %j.1.lcssa = phi i32 [ 0, %for.end17 ], [ %j.2, %while.end.loopexit ], [ 0, %for.end ]
  %i.1.lcssa = phi i32 [ 0, %for.end17 ], [ %i.2, %while.end.loopexit ], [ 0, %for.end ]
  %cmp37105 = icmp slt i32 %sub, %i.1.lcssa
  br i1 %cmp37105, label %while.end45, label %while.body38.lr.ph

while.body38.lr.ph:                               ; preds = %while.end
  %71 = sext i32 %i.1.lcssa to i64
  %72 = sext i32 %sub to i64
  %73 = sext i32 %k.0.lcssa to i64
  %scevgep128 = getelementptr i32, i32* %arr, i64 %73
  %scevgep128129 = bitcast i32* %scevgep128 to i8*
  %scevgep130 = getelementptr i32, i32* %vla, i64 %71
  %scevgep130131 = bitcast i32* %scevgep130 to i8*
  %74 = sub i32 %sub, %i.1.lcssa
  %75 = zext i32 %74 to i64
  %76 = shl nuw nsw i64 %75, 2
  %77 = add nuw nsw i64 %76, 4
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %scevgep128129, i8* %scevgep130131, i64 %77, i32 4, i1 false)
  %78 = icmp sgt i64 %72, %71
  %smax = select i1 %78, i64 %72, i64 %71
  %79 = add nsw i64 %smax, 1
  %80 = sub nsw i64 %79, %71
  %min.iters.check151 = icmp ult i64 %80, 4
  br i1 %min.iters.check151, label %while.body38.preheader, label %vector.ph152

vector.ph152:                                     ; preds = %while.body38.lr.ph
  %n.vec154 = and i64 %80, -4
  %ind.end = add nsw i64 %n.vec154, %71
  %81 = insertelement <2 x i64> <i64 undef, i64 0>, i64 %73, i32 0
  %82 = add nsw i64 %n.vec154, -4
  %83 = lshr exact i64 %82, 2
  %84 = add nuw nsw i64 %83, 1
  %xtraiter = and i64 %84, 7
  %85 = icmp ult i64 %82, 28
  br i1 %85, label %middle.block149.unr-lcssa, label %vector.ph152.new

vector.ph152.new:                                 ; preds = %vector.ph152
  %unroll_iter = sub nsw i64 %84, %xtraiter
  br label %vector.body148

vector.body148:                                   ; preds = %vector.body148, %vector.ph152.new
  %vec.phi = phi <2 x i64> [ %81, %vector.ph152.new ], [ %86, %vector.body148 ]
  %vec.phi159 = phi <2 x i64> [ zeroinitializer, %vector.ph152.new ], [ %87, %vector.body148 ]
  %niter = phi i64 [ %unroll_iter, %vector.ph152.new ], [ %niter.nsub.7, %vector.body148 ]
  %86 = add nsw <2 x i64> %vec.phi, <i64 8, i64 8>
  %87 = add nuw nsw <2 x i64> %vec.phi159, <i64 8, i64 8>
  %niter.nsub.7 = add i64 %niter, -8
  %niter.ncmp.7 = icmp eq i64 %niter.nsub.7, 0
  br i1 %niter.ncmp.7, label %middle.block149.unr-lcssa, label %vector.body148, !llvm.loop !11

middle.block149.unr-lcssa:                        ; preds = %vector.body148, %vector.ph152
  %.lcssa166.ph = phi <2 x i64> [ undef, %vector.ph152 ], [ %86, %vector.body148 ]
  %.lcssa.ph = phi <2 x i64> [ undef, %vector.ph152 ], [ %87, %vector.body148 ]
  %vec.phi.unr = phi <2 x i64> [ %81, %vector.ph152 ], [ %86, %vector.body148 ]
  %vec.phi159.unr = phi <2 x i64> [ zeroinitializer, %vector.ph152 ], [ %87, %vector.body148 ]
  %lcmp.mod = icmp eq i64 %xtraiter, 0
  br i1 %lcmp.mod, label %middle.block149, label %vector.body148.epil.preheader

vector.body148.epil.preheader:                    ; preds = %middle.block149.unr-lcssa
  br label %vector.body148.epil

vector.body148.epil:                              ; preds = %vector.body148.epil, %vector.body148.epil.preheader
  %vec.phi.epil = phi <2 x i64> [ %vec.phi.unr, %vector.body148.epil.preheader ], [ %88, %vector.body148.epil ]
  %vec.phi159.epil = phi <2 x i64> [ %vec.phi159.unr, %vector.body148.epil.preheader ], [ %89, %vector.body148.epil ]
  %epil.iter = phi i64 [ %xtraiter, %vector.body148.epil.preheader ], [ %epil.iter.sub, %vector.body148.epil ]
  %88 = add nsw <2 x i64> %vec.phi.epil, <i64 1, i64 1>
  %89 = add nuw nsw <2 x i64> %vec.phi159.epil, <i64 1, i64 1>
  %epil.iter.sub = add i64 %epil.iter, -1
  %epil.iter.cmp = icmp eq i64 %epil.iter.sub, 0
  br i1 %epil.iter.cmp, label %middle.block149, label %vector.body148.epil, !llvm.loop !12

middle.block149:                                  ; preds = %vector.body148.epil, %middle.block149.unr-lcssa
  %.lcssa166 = phi <2 x i64> [ %.lcssa166.ph, %middle.block149.unr-lcssa ], [ %88, %vector.body148.epil ]
  %.lcssa = phi <2 x i64> [ %.lcssa.ph, %middle.block149.unr-lcssa ], [ %89, %vector.body148.epil ]
  %bin.rdx = add <2 x i64> %.lcssa, %.lcssa166
  %rdx.shuf = shufflevector <2 x i64> %bin.rdx, <2 x i64> undef, <2 x i32> <i32 1, i32 undef>
  %bin.rdx164 = add <2 x i64> %bin.rdx, %rdx.shuf
  %90 = extractelement <2 x i64> %bin.rdx164, i32 0
  %cmp.n158 = icmp eq i64 %80, %n.vec154
  br i1 %cmp.n158, label %while.end45.loopexit, label %while.body38.preheader

while.body38.preheader:                           ; preds = %middle.block149, %while.body38.lr.ph
  %indvars.iv126.ph = phi i64 [ %73, %while.body38.lr.ph ], [ %90, %middle.block149 ]
  %indvars.iv.ph = phi i64 [ %71, %while.body38.lr.ph ], [ %ind.end, %middle.block149 ]
  br label %while.body38

while.body38:                                     ; preds = %while.body38.preheader, %while.body38
  %indvars.iv126 = phi i64 [ %indvars.iv.next127, %while.body38 ], [ %indvars.iv126.ph, %while.body38.preheader ]
  %indvars.iv = phi i64 [ %indvars.iv.next, %while.body38 ], [ %indvars.iv.ph, %while.body38.preheader ]
  %indvars.iv.next = add nsw i64 %indvars.iv, 1
  %indvars.iv.next127 = add nsw i64 %indvars.iv126, 1
  %cmp37 = icmp slt i64 %indvars.iv, %72
  br i1 %cmp37, label %while.body38, label %while.end45.loopexit, !llvm.loop !13

while.end45.loopexit:                             ; preds = %while.body38, %middle.block149
  %indvars.iv.next127.lcssa = phi i64 [ %90, %middle.block149 ], [ %indvars.iv.next127, %while.body38 ]
  %91 = trunc i64 %indvars.iv.next127.lcssa to i32
  br label %while.end45

while.end45:                                      ; preds = %while.end45.loopexit, %while.end
  %k.1.lcssa = phi i32 [ %k.0.lcssa, %while.end ], [ %91, %while.end45.loopexit ]
  %cmp47102 = icmp slt i32 %j.1.lcssa, %sub1
  br i1 %cmp47102, label %while.body48.lr.ph, label %while.end55

while.body48.lr.ph:                               ; preds = %while.end45
  %92 = sext i32 %j.1.lcssa to i64
  %93 = sext i32 %k.1.lcssa to i64
  %scevgep = getelementptr i32, i32* %arr, i64 %93
  %scevgep123 = bitcast i32* %scevgep to i8*
  %scevgep124 = getelementptr i32, i32* %vla2, i64 %92
  %scevgep124125 = bitcast i32* %scevgep124 to i8*
  %94 = add i32 %r, -1
  %95 = sub i32 %94, %j.1.lcssa
  %96 = sub i32 %95, %m
  %97 = zext i32 %96 to i64
  %98 = shl nuw nsw i64 %97, 2
  %99 = add nuw nsw i64 %98, 4
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %scevgep123, i8* %scevgep124125, i64 %99, i32 4, i1 false)
  br label %while.end55

while.end55:                                      ; preds = %while.body48.lr.ph, %while.end45
  ret void
}

; Function Attrs: nounwind uwtable
define void @mergeSort(i32* %arr, i32 %l, i32 %r) local_unnamed_addr #0 {
entry:
  %cmp = icmp sgt i32 %r, %l
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %sub = sub nsw i32 %r, %l
  %div = sdiv i32 %sub, 2
  %add = add nsw i32 %div, %l
  tail call void @mergeSort(i32* %arr, i32 %l, i32 %add)
  %add1 = add nsw i32 %add, 1
  tail call void @mergeSort(i32* %arr, i32 %add1, i32 %r)
  tail call void @merge(i32* %arr, i32 %l, i32 %add, i32 %r)
  ret void

if.end:                                           ; preds = %entry
  ret void
}

; Function Attrs: nounwind uwtable
define void @printArray(i32* nocapture readonly %A, i32 %size) local_unnamed_addr #0 {
entry:
  %cmp5 = icmp sgt i32 %size, 0
  br i1 %cmp5, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %entry
  %wide.trip.count = zext i32 %size to i64
  br label %for.body

for.body:                                         ; preds = %for.body, %for.body.lr.ph
  %indvars.iv = phi i64 [ 0, %for.body.lr.ph ], [ %indvars.iv.next, %for.body ]
  %arrayidx = getelementptr inbounds i32, i32* %A, i64 %indvars.iv
  %0 = load i32, i32* %arrayidx, align 4, !tbaa !2
  %call = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %0)
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, %wide.trip.count
  br i1 %exitcond, label %for.end, label %for.body

for.end:                                          ; preds = %for.body, %entry
  %putchar = tail call i32 @putchar(i32 10)
  ret void
}

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #4

; Function Attrs: nounwind uwtable
define i32 @main() local_unnamed_addr #0 {
entry:
  %arr = alloca [6 x i32], align 16
  %0 = bitcast [6 x i32]* %arr to i8*
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %0) #2
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull %0, i8* bitcast ([6 x i32]* @main.arr to i8*), i64 24, i32 16, i1 false)
  %puts = tail call i32 @puts(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i64 0, i64 0))
  %arrayidx.i = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i64 0, i64 0
  %call.i = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 12) #2
  %arrayidx.i.1 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i64 0, i64 1
  %call.i.1 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 11) #2
  %arrayidx.i.2 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i64 0, i64 2
  %call.i.2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 13) #2
  %arrayidx.i.3 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i64 0, i64 3
  %call.i.3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 5) #2
  %arrayidx.i.4 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i64 0, i64 4
  %call.i.4 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 6) #2
  %arrayidx.i.5 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i64 0, i64 5
  %call.i.5 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 7) #2
  %putchar.i = tail call i32 @putchar(i32 10) #2
  call void @mergeSort(i32* nonnull %arrayidx.i, i32 0, i32 5)
  %puts7 = call i32 @puts(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.4, i64 0, i64 0))
  %1 = load i32, i32* %arrayidx.i, align 16, !tbaa !2
  %call.i10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %1) #2
  %2 = load i32, i32* %arrayidx.i.1, align 4, !tbaa !2
  %call.i10.1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %2) #2
  %3 = load i32, i32* %arrayidx.i.2, align 8, !tbaa !2
  %call.i10.2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %3) #2
  %4 = load i32, i32* %arrayidx.i.3, align 4, !tbaa !2
  %call.i10.3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %4) #2
  %5 = load i32, i32* %arrayidx.i.4, align 16, !tbaa !2
  %call.i10.4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %5) #2
  %6 = load i32, i32* %arrayidx.i.5, align 4, !tbaa !2
  %call.i10.5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %6) #2
  %putchar.i14 = call i32 @putchar(i32 10) #2
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %0) #2
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: nounwind
declare i32 @putchar(i32) local_unnamed_addr #2

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #2

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind }
attributes #3 = { norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.isvectorized", i32 1}
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.unroll.disable"}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !9}
!13 = distinct !{!13, !14, !7}
!14 = !{!"llvm.loop.unroll.runtime.disable"}
