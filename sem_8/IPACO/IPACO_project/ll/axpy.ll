; ModuleID = 'axpy.cu'
source_filename = "axpy.cu"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

module asm ".globl _ZSt21ios_base_library_initv"

%"class.std::basic_ostream" = type { ptr, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", ptr, i8, i8, ptr, ptr, ptr, ptr }
%"class.std::ios_base" = type { ptr, i64, i64, i32, i32, i32, ptr, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, ptr, %"class.std::locale" }
%"struct.std::ios_base::_Words" = type { ptr, i64 }
%"class.std::locale" = type { ptr }
%struct.dim3 = type { i32, i32, i32 }

$_ZN4dim3C2Ejjj = comdat any

@__const.main.host_x = private unnamed_addr constant [4 x float] [float 1.000000e+00, float 2.000000e+00, float 3.000000e+00, float 4.000000e+00], align 16
@_ZSt4cout = external global %"class.std::basic_ostream", align 8
@.str = private unnamed_addr constant [3 x i8] c"y[\00", align 1
@.str.1 = private unnamed_addr constant [5 x i8] c"] = \00", align 1
@.str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local void @_Z19__device_stub__axpyfPfS_(float noundef %a, ptr noundef %x, ptr noundef %y) #0 {
entry:
  %a.addr = alloca float, align 4
  %x.addr = alloca ptr, align 8
  %y.addr = alloca ptr, align 8
  %grid_dim = alloca %struct.dim3, align 8
  %block_dim = alloca %struct.dim3, align 8
  %shmem_size = alloca i64, align 8
  %stream = alloca ptr, align 8
  %grid_dim.coerce = alloca { i64, i32 }, align 8
  %block_dim.coerce = alloca { i64, i32 }, align 8
  store float %a, ptr %a.addr, align 4
  store ptr %x, ptr %x.addr, align 8
  store ptr %y, ptr %y.addr, align 8
  %kernel_args = alloca ptr, i64 3, align 16
  %0 = getelementptr ptr, ptr %kernel_args, i32 0
  store ptr %a.addr, ptr %0, align 8
  %1 = getelementptr ptr, ptr %kernel_args, i32 1
  store ptr %x.addr, ptr %1, align 8
  %2 = getelementptr ptr, ptr %kernel_args, i32 2
  store ptr %y.addr, ptr %2, align 8
  %3 = call i32 @__cudaPopCallConfiguration(ptr %grid_dim, ptr %block_dim, ptr %shmem_size, ptr %stream)
  %4 = load i64, ptr %shmem_size, align 8
  %5 = load ptr, ptr %stream, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %grid_dim.coerce, ptr align 8 %grid_dim, i64 12, i1 false)
  %6 = getelementptr inbounds nuw { i64, i32 }, ptr %grid_dim.coerce, i32 0, i32 0
  %7 = load i64, ptr %6, align 8
  %8 = getelementptr inbounds nuw { i64, i32 }, ptr %grid_dim.coerce, i32 0, i32 1
  %9 = load i32, ptr %8, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %block_dim.coerce, ptr align 8 %block_dim, i64 12, i1 false)
  %10 = getelementptr inbounds nuw { i64, i32 }, ptr %block_dim.coerce, i32 0, i32 0
  %11 = load i64, ptr %10, align 8
  %12 = getelementptr inbounds nuw { i64, i32 }, ptr %block_dim.coerce, i32 0, i32 1
  %13 = load i32, ptr %12, align 8
  %call = call noundef i32 @cudaLaunchKernel(ptr noundef @_Z19__device_stub__axpyfPfS_, i64 %7, i32 %9, i64 %11, i32 %13, ptr noundef %kernel_args, i64 noundef %4, ptr noundef %5)
  br label %setup.end

setup.end:                                        ; preds = %entry
  ret void
}

declare i32 @__cudaPopCallConfiguration(ptr, ptr, ptr, ptr)

declare i32 @cudaLaunchKernel(ptr, i64, i32, i64, i32, ptr, i64, ptr)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #1

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local noundef i32 @main(i32 noundef %argc, ptr noundef %argv) #2 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca ptr, align 8
  %kDataLen = alloca i32, align 4
  %a = alloca float, align 4
  %host_x = alloca [4 x float], align 16
  %host_y = alloca [4 x float], align 16
  %device_x = alloca ptr, align 8
  %device_y = alloca ptr, align 8
  %agg.tmp = alloca %struct.dim3, align 4
  %agg.tmp3 = alloca %struct.dim3, align 4
  %agg.tmp.coerce = alloca { i64, i32 }, align 4
  %agg.tmp3.coerce = alloca { i64, i32 }, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 %argc, ptr %argc.addr, align 4
  store ptr %argv, ptr %argv.addr, align 8
  store i32 4, ptr %kDataLen, align 4
  store float 2.000000e+00, ptr %a, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %host_x, ptr align 16 @__const.main.host_x, i64 16, i1 false)
  %call = call noundef i32 @_ZL10cudaMallocIfE9cudaErrorPPT_m(ptr noundef %device_x, i64 noundef 16)
  %call1 = call noundef i32 @_ZL10cudaMallocIfE9cudaErrorPPT_m(ptr noundef %device_y, i64 noundef 16)
  %0 = load ptr, ptr %device_x, align 8
  %arraydecay = getelementptr inbounds [4 x float], ptr %host_x, i64 0, i64 0
  %call2 = call i32 @cudaMemcpy(ptr noundef %0, ptr noundef %arraydecay, i64 noundef 16, i32 noundef 1)
  call void @_ZN4dim3C2Ejjj(ptr noundef nonnull align 4 dereferenceable(12) %agg.tmp, i32 noundef 1, i32 noundef 1, i32 noundef 1)
  call void @_ZN4dim3C2Ejjj(ptr noundef nonnull align 4 dereferenceable(12) %agg.tmp3, i32 noundef 4, i32 noundef 1, i32 noundef 1)
  call void @llvm.memcpy.p0.p0.i64(ptr align 4 %agg.tmp.coerce, ptr align 4 %agg.tmp, i64 12, i1 false)
  %1 = getelementptr inbounds nuw { i64, i32 }, ptr %agg.tmp.coerce, i32 0, i32 0
  %2 = load i64, ptr %1, align 4
  %3 = getelementptr inbounds nuw { i64, i32 }, ptr %agg.tmp.coerce, i32 0, i32 1
  %4 = load i32, ptr %3, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 4 %agg.tmp3.coerce, ptr align 4 %agg.tmp3, i64 12, i1 false)
  %5 = getelementptr inbounds nuw { i64, i32 }, ptr %agg.tmp3.coerce, i32 0, i32 0
  %6 = load i64, ptr %5, align 4
  %7 = getelementptr inbounds nuw { i64, i32 }, ptr %agg.tmp3.coerce, i32 0, i32 1
  %8 = load i32, ptr %7, align 4
  %call4 = call i32 @__cudaPushCallConfiguration(i64 %2, i32 %4, i64 %6, i32 %8, i64 noundef 0, ptr noundef null)
  %tobool = icmp ne i32 %call4, 0
  br i1 %tobool, label %kcall.end, label %kcall.configok

kcall.configok:                                   ; preds = %entry
  %9 = load float, ptr %a, align 4
  %10 = load ptr, ptr %device_x, align 8
  %11 = load ptr, ptr %device_y, align 8
  call void @_Z19__device_stub__axpyfPfS_(float noundef %9, ptr noundef %10, ptr noundef %11) #6
  br label %kcall.end

kcall.end:                                        ; preds = %kcall.configok, %entry
  %call5 = call i32 @cudaDeviceSynchronize()
  %arraydecay6 = getelementptr inbounds [4 x float], ptr %host_y, i64 0, i64 0
  %12 = load ptr, ptr %device_y, align 8
  %call7 = call i32 @cudaMemcpy(ptr noundef %arraydecay6, ptr noundef %12, i64 noundef 16, i32 noundef 2)
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %kcall.end
  %13 = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %13, 4
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %call8 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, ptr noundef @.str)
  %14 = load i32, ptr %i, align 4
  %call9 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8) %call8, i32 noundef %14)
  %call10 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8) %call9, ptr noundef @.str.1)
  %15 = load i32, ptr %i, align 4
  %idxprom = sext i32 %15 to i64
  %arrayidx = getelementptr inbounds [4 x float], ptr %host_y, i64 0, i64 %idxprom
  %16 = load float, ptr %arrayidx, align 4
  %call11 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEf(ptr noundef nonnull align 8 dereferenceable(8) %call10, float noundef %16)
  %call12 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8) %call11, ptr noundef @.str.2)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %17 = load i32, ptr %i, align 4
  %inc = add nsw i32 %17, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !7

for.end:                                          ; preds = %for.cond
  %call13 = call i32 @cudaDeviceReset()
  ret i32 0
}

; Function Attrs: mustprogress noinline optnone uwtable
define internal noundef i32 @_ZL10cudaMallocIfE9cudaErrorPPT_m(ptr noundef %devPtr, i64 noundef %size) #3 {
entry:
  %devPtr.addr = alloca ptr, align 8
  %size.addr = alloca i64, align 8
  store ptr %devPtr, ptr %devPtr.addr, align 8
  store i64 %size, ptr %size.addr, align 8
  %0 = load ptr, ptr %devPtr.addr, align 8
  %1 = load i64, ptr %size.addr, align 8
  %call = call i32 @cudaMalloc(ptr noundef %0, i64 noundef %1)
  ret i32 %call
}

declare i32 @cudaMemcpy(ptr noundef, ptr noundef, i64 noundef, i32 noundef) #4

declare i32 @__cudaPushCallConfiguration(i64, i32, i64, i32, i64 noundef, ptr noundef) #4

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN4dim3C2Ejjj(ptr noundef nonnull align 4 dereferenceable(12) %this, i32 noundef %vx, i32 noundef %vy, i32 noundef %vz) unnamed_addr #5 comdat align 2 {
entry:
  %this.addr = alloca ptr, align 8
  %vx.addr = alloca i32, align 4
  %vy.addr = alloca i32, align 4
  %vz.addr = alloca i32, align 4
  store ptr %this, ptr %this.addr, align 8
  store i32 %vx, ptr %vx.addr, align 4
  store i32 %vy, ptr %vy.addr, align 4
  store i32 %vz, ptr %vz.addr, align 4
  %this1 = load ptr, ptr %this.addr, align 8
  %x = getelementptr inbounds nuw %struct.dim3, ptr %this1, i32 0, i32 0
  %0 = load i32, ptr %vx.addr, align 4
  store i32 %0, ptr %x, align 4
  %y = getelementptr inbounds nuw %struct.dim3, ptr %this1, i32 0, i32 1
  %1 = load i32, ptr %vy.addr, align 4
  store i32 %1, ptr %y, align 4
  %z = getelementptr inbounds nuw %struct.dim3, ptr %this1, i32 0, i32 2
  %2 = load i32, ptr %vz.addr, align 4
  store i32 %2, ptr %z, align 4
  ret void
}

declare i32 @cudaDeviceSynchronize() #4

declare noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8), ptr noundef) #4

declare noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8), i32 noundef) #4

declare noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEf(ptr noundef nonnull align 8 dereferenceable(8), float noundef) #4

declare i32 @cudaDeviceReset() #4

declare i32 @cudaMalloc(ptr noundef, i64 noundef) #4

attributes #0 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "uniform-work-group-size"="true" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { "uniform-work-group-size"="true" }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.ident = !{!6}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 12, i32 8]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"PIE Level", i32 2}
!4 = !{i32 7, !"uwtable", i32 2}
!5 = !{i32 7, !"frame-pointer", i32 2}
!6 = !{!"clang version 21.0.0git (https://github.com/llvm/llvm-project.git f39696e7dee4f1dce8c10d2b17f987643c480895)"}
!7 = distinct !{!7, !8}
!8 = !{!"llvm.loop.mustprogress"}
; ModuleID = 'axpy.cu'
source_filename = "axpy.cu"
target datalayout = "e-p6:32:32-i64:64-i128:128-v16:16-v32:32-n16:32:64"
target triple = "nvptx64-nvidia-cuda"

%struct.__cuda_builtin_threadIdx_t = type { i8 }

@threadIdx = extern_weak dso_local addrspace(1) global %struct.__cuda_builtin_threadIdx_t, align 1

; Function Attrs: convergent mustprogress noinline norecurse nounwind optnone
define dso_local ptx_kernel void @_Z4axpyfPfS_(float noundef %a, ptr noundef %x, ptr noundef %y) #0 {
entry:
  %a.addr = alloca float, align 4
  %x.addr = alloca ptr, align 8
  %y.addr = alloca ptr, align 8
  store float %a, ptr %a.addr, align 4
  store ptr %x, ptr %x.addr, align 8
  store ptr %y, ptr %y.addr, align 8
  %0 = load float, ptr %a.addr, align 4
  %1 = load ptr, ptr %x.addr, align 8
  %2 = call noundef range(i32 0, 1024) i32 @llvm.nvvm.read.ptx.sreg.tid.x()
  %idxprom = zext i32 %2 to i64
  %arrayidx = getelementptr inbounds nuw float, ptr %1, i64 %idxprom
  %3 = load float, ptr %arrayidx, align 4
  %mul = fmul contract float %0, %3
  %4 = load ptr, ptr %y.addr, align 8
  %5 = call noundef range(i32 0, 1024) i32 @llvm.nvvm.read.ptx.sreg.tid.x()
  %idxprom2 = zext i32 %5 to i64
  %arrayidx3 = getelementptr inbounds nuw float, ptr %4, i64 %idxprom2
  store float %mul, ptr %arrayidx3, align 4
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare noundef i32 @llvm.nvvm.read.ptx.sreg.tid.x() #1

attributes #0 = { convergent mustprogress noinline norecurse nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="sm_75" "target-features"="+ptx87,+sm_75" "uniform-work-group-size"="true" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!llvm.module.flags = !{!0, !1, !2, !3}
!nvvm.annotations = !{!4}
!llvm.ident = !{!5, !6}
!nvvmir.version = !{!7}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 12, i32 8]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 4, !"nvvm-reflect-ftz", i32 0}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{ptr @_Z4axpyfPfS_}
!5 = !{!"clang version 21.0.0git (https://github.com/llvm/llvm-project.git f39696e7dee4f1dce8c10d2b17f987643c480895)"}
!6 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
!7 = !{i32 2, i32 0}
