#include "matrix.h"

#ifndef FINAL_MPI_H
#define FINAL_MPI_H

#define MASTER 0   /* taskid of first task */

extern void multiply(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int size, int quantity);

void master_routine(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int rank, int world_size, int size);

void worker_routine(int rank, int world_size, int size);

void cuda_master_routine(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int rank, int world_size, int size);

void cuda_worker_routine(int rank, int world_size, int size);

#endif //FINAL_MPI_H
