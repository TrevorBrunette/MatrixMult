#include <mpi.h>
#include "matrix_mpi.h"

void master_routine(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int rank, int world_size, int size) {
    int worker_count = world_size - 1;
    int rows_per_worker = size/worker_count;
    int leftovers = size%worker_count;
    int offset = 0;
    int quantity = 0;
    for(int dest = 1; dest < world_size; ++dest) {
        quantity = rows_per_worker;
        if(dest <= leftovers) {
            quantity += 1;
        }
        MPI_Request request1, request2, request3, request4;
        MPI_Isend(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, &request3);
        MPI_Isend(&quantity, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, &request4);
        MPI_Isend(&matrix1[offset*size], quantity*size, MPI_DOUBLE, dest, 1,MPI_COMM_WORLD, &request1);
        MPI_Isend(matrix2, size*size, MPI_DOUBLE, dest, 1,MPI_COMM_WORLD, &request2);
        offset += rows_per_worker;
        if(dest <= leftovers) {
            offset += 1;
        }
    }

    MPI_Status stat1, stat2, stat3;
    for(int dest = 1; dest < world_size; ++dest) {
        MPI_Recv(&offset, 1, MPI_INT, dest, 2, MPI_COMM_WORLD, &stat1);
        MPI_Recv(&quantity, 1, MPI_INT, dest, 2, MPI_COMM_WORLD, &stat2);
        MPI_Recv(&matrix3[offset * size], quantity * size, MPI_DOUBLE, dest, 2, MPI_COMM_WORLD, &stat3);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

void worker_routine(int rank, int world_size, int size) {
    int offset;
    int quantity;
    Datatype* matrix1;
    Datatype* matrix2;
    Datatype* matrix3;

    MPI_Status status1, status2, status3, status4;
    MPI_Recv(&offset, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &status3);
    MPI_Recv(&quantity, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &status4);

    matrix1 = initSlice(size * quantity);
    matrix2 = initMatrix(size);
    matrix3 = initSlice(size * quantity);

    MPI_Recv(matrix1, quantity*size, MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD, &status1);
    MPI_Recv(matrix2, size*size, MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD, &status2);

    int worker_count = world_size - 1;
    int rows_per_worker = size/worker_count;

    for(int row = offset; row < (offset+quantity); ++row) {
        for(int col = 0; col < size; ++col) {
            for(int k = 0; k < size; ++k) {
                matrix3[(row-offset) * size + col] += matrix1[(row-offset) * size + k] * matrix2[k * size + col];
            }
        }
    }

    MPI_Send(&offset, 1, MPI_INT, MASTER, 2, MPI_COMM_WORLD);
    MPI_Send(&quantity, 1, MPI_INT, MASTER, 2, MPI_COMM_WORLD);
    MPI_Send(matrix3, size*quantity, MPI_DOUBLE, MASTER, 2, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
}

void cuda_master_routine(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int rank, int world_size, int size) {
    int worker_count = world_size - 1;
    int rows_per_worker = size/worker_count;
    int leftovers = size%worker_count;
    int offset = 0;
    int quantity = 0;
    for(int dest = 1; dest < world_size; ++dest) {
        quantity = rows_per_worker;
        if(dest <= leftovers) {
            quantity += 1;
        }
        MPI_Request request1, request2, request3, request4;
        MPI_Isend(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, &request3);
        MPI_Isend(&quantity, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, &request4);
        MPI_Isend(&matrix1[offset*size], quantity*size, MPI_DOUBLE, dest, 1,MPI_COMM_WORLD, &request1);
        MPI_Isend(matrix2, size*size, MPI_DOUBLE, dest, 1,MPI_COMM_WORLD, &request2);
        offset += rows_per_worker;
        if(dest <= leftovers) {
            offset += 1;
        }
    }

    MPI_Status stat1, stat2, stat3;
    for(int dest = 1; dest < world_size; ++dest) {
        MPI_Recv(&offset, 1, MPI_INT, dest, 2, MPI_COMM_WORLD, &stat1);
        MPI_Recv(&quantity, 1, MPI_INT, dest, 2, MPI_COMM_WORLD, &stat2);
        MPI_Recv(&matrix3[offset * size], quantity * size, MPI_DOUBLE, dest, 2, MPI_COMM_WORLD, &stat3);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

void cuda_worker_routine(int rank, int world_size, int size) {
    int offset;
    int quantity;
    Datatype* matrix1;
    Datatype* matrix2;
    Datatype* matrix3;

    MPI_Status status1, status2, status3, status4;
    MPI_Recv(&offset, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &status3);
    MPI_Recv(&quantity, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &status4);

    matrix1 = initSlice(size * quantity);
    matrix2 = initMatrix(size);
    matrix3 = initSlice(size * quantity);

    MPI_Recv(matrix1, quantity*size, MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD, &status1);
    MPI_Recv(matrix2, size*size, MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD, &status2);

    int worker_count = world_size - 1;
    int rows_per_worker = size/worker_count;

    for(int row = offset; row < (offset+quantity); ++row) {
        for(int col = 0; col < size; ++col) {
            for(int k = 0; k < size; ++k) {
                matrix3[(row-offset) * size + col] += matrix1[(row-offset) * size + k] * matrix2[k * size + col];
            }
        }
    }

    MPI_Send(&offset, 1, MPI_INT, MASTER, 2, MPI_COMM_WORLD);
    MPI_Send(&quantity, 1, MPI_INT, MASTER, 2, MPI_COMM_WORLD);
    MPI_Send(matrix3, size*quantity, MPI_DOUBLE, MASTER, 2, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
}