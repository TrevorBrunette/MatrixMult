#include <stdio.h>
#include "matrix.h"

void printMatrix(const Datatype* matrix, int size) {
    int size2 = size * size;
    for(int i = 0; i < size2; ++i){
        printf("%12.2lf ", matrix[i]);
        if(i % size == size - 1) printf("\n");
    }
    printf("\n");
}

 __global__ void multiplyMatrix(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int size, int quantity, int offset){
    int thread_index = blockIdx.x * blockDim.x + threadIdx.x;
    if(thread_index >= size*quantity){
        return;
    }
    // index = y * size + x;
    int x = thread_index % size;
    int y = thread_index / size;
    double sum = 0;
    for(int k = 0; k < size; ++k){
        sum += matrix1[y*size + k] * matrix2[k*size + x];
    }
    matrix3[thread_index] = sum;
 }

extern "C" void multiply(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int size, int quantity, int offset) {
    Datatype* working1;
    Datatype* working2;
    Datatype* working3;
    cudaMalloc((void **)&working1, size*quantity*sizeof(Datatype));
    cudaMalloc((void **)&working2, size*size*sizeof(Datatype));
    cudaMalloc((void **)&working3, size*quantity*sizeof(Datatype));


    cudaMemcpy(working1, matrix1, size*quantity*sizeof(Datatype), cudaMemcpyHostToDevice);
    cudaMemcpy(working2, matrix2, size*size*sizeof(Datatype), cudaMemcpyHostToDevice);

    int block_count = (size*quantity) / 32 + 1;
    multiplyMatrix<<<block_count,32>>>(working1, working2, working3, size, quantity, offset);
    // multiplyMatrix<<<block_count,32>>>(matrix1, matrix2, matrix3, size, quantity, offset);

    cudaDeviceSynchronize();
    cudaMemcpy(matrix3, working3, size*quantity*sizeof(Datatype), cudaMemcpyDeviceToHost);

    cudaFree(working1);
    cudaFree(working2);
    cudaFree(working3);
}
