#include <stdio.h>
#include "matrix.h"

 __global__ void multiplyMatrix(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int size, int quantity, int offset){
    int thread_index = blockIdx.x * blockDim.x + threadIdx.x;
    if(thread_index < size*quantity){
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



extern "C" void initCuda(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int size, int rank, int quantity) {
    //rank should be decremented by one
    int cudaDeviceCount = 0;
    int cE;
    //Error checking
    if( (cE = cudaGetDeviceCount( &cudaDeviceCount)) != cudaSuccess )
    {
        printf(" Unable to determine cuda device count, error is %d, count is %d\n",
               cE, cudaDeviceCount );
        exit(-1);
    }
    if( (cE = cudaSetDevice( rank % cudaDeviceCount )) != cudaSuccess )
    {
        printf(" Unable to have rank %d set to cuda device %d, error is %d \n",
               rank, (rank % cudaDeviceCount), cE);
        exit(-1);
    }

    cudaMallocManaged(&matrix1, size*quantity*sizeof(Datatype));
    cudaMallocManaged(&matrix2, size*size*sizeof(Datatype));
    cudaMallocManaged(&matrix3, size*size*sizeof(Datatype));

}

extern "C" void multiply(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int size, int quantity, int offset) {

    int block_count = (size*quantity) / 32 + 1;
    multiplyMatrix<<<block_count,32>>>(matrix1, matrix2, matrix3, size, quantity, offset);
}
