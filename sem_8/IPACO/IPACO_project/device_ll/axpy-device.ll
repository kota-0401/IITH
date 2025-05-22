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
