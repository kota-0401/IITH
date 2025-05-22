#include <stdio.h>
#include <cuda.h>

__global__ void kernel(int *dA, int *dB, int *dC, int n) {
    int sum = 0;
    for (int k = 0; k < n; k++) {
        sum += (dA[threadIdx.x * n + k] * dB[k * blockDim.y + threadIdx.y]);
    }
    dC[threadIdx.x * blockDim.y + threadIdx.y] = sum;
}

void read_matrix(FILE* file, int *A, int *B, int m, int n, int l) {
    // Read Matrix A
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(file, "%d", &A[i * n + j]);
        }
    }

    // Read Matrix B
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < l; j++) {
            fscanf(file, "%d", &B[i * l + j]);
        }
    }
}

int main() {
    FILE* input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        printf("Error opening input.txt\n");
        return 1;
    }

    int m, n, l;
    fscanf(input_file, "%d %d", &m, &n);
    fscanf(input_file, "%d %d", &n, &l);

    int *A = (int *)malloc(m * n * sizeof(int));
    int *B = (int *)malloc(n * l * sizeof(int));
    int *C = (int *)malloc(m * l * sizeof(int));

    read_matrix(input_file, A, B, m, n, l);
    fclose(input_file);

    int *dA, *dB, *dC;
    cudaMalloc(&dA, m * n * sizeof(int));
    cudaMalloc(&dB, n * l * sizeof(int));
    cudaMalloc(&dC, m * l * sizeof(int));

    // Memory copy from host to device
    cudaMemcpy(dA, A, m * n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dB, B, n * l * sizeof(int), cudaMemcpyHostToDevice);

    free(A);
    free(B);

    dim3 block(m, l, 1);
    
    // Launching the GPU kernel
    kernel<<<1, block>>>(dA, dB, dC, n);

    // Memory copy from device to host (Also Synchronizing)
    cudaMemcpy(C, dC, m * l * sizeof(int), cudaMemcpyDeviceToHost);

    FILE* output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Error opening output.txt\n");
        return 1;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++) {
            fprintf(output_file, "%d ", C[i * l + j]);
        }
        fprintf(output_file, "\n");
    }
    fclose(output_file);
    free(C);

    return 0;
}