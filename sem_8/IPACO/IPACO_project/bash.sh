nvcc axpy.cu -o axpy -arch=sm_75

clang++ axpy.cu -o axpy --cuda-gpu-arch=sm_75 -L/usr/local/cuda-12.8/lib64 -lcudart_static -ldl -lrt -pthread

mkdir output
clang++ -x cuda --cuda-host-only -S -emit-llvm -o host_ll/file1-host.ll input/file1.cu
clang++ -O0 -g -Xclang -disable-O0-optnone -x cuda --cuda-device-only --cuda-gpu-arch=sm_75 -S -emit-llvm -o device_ll/file1-device.ll input/file1.cu
clang++ -x cuda --cuda-device-only --cuda-gpu-arch=sm_75 -S -emit-llvm -o device_ll/file1-device.ll input/file1.cu

cat host_ll/file1-host.ll device_ll/file1-device.ll > ll/file1.ll

ninja CudaKernelPass

llvm-project/build/bin/opt -load-pass-plugin llvm-project/build/lib/CudaKernelPass.so -passes="cuda-kernel-pass" -disable-output device_ll/file1-device.ll

Transformed ll:
llvm-project/build/bin/opt -load-pass-plugin llvm-project/build/lib/CudaKernelPass.so -passes="cuda-kernel-pass" device_ll/file1-device.ll -o output/file1.txt3

$ llvm-project/build/bin/opt -load-pass-plugin llvm-project/build/lib/CudaKernelPass.so -passes="cuda-kernel-pass" -disable-output device_ll/file1-device.ll 2>&1

$ llvm-project/build/bin/opt -load-pass-plugin llvm-project/build/lib/CudaKernelPass.so -passes="cuda-kernel-pass" -disable-output device_ll/file1-device.ll 2> race_output.txt
