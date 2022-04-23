#include "main.h"

#define MASTER 0   // taskid of first task

int main(int argc, char** argv) {
    const int size = 200;

    int rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size < 2 ) {
        printf("Need at least two MPI tasks. Quitting.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }

    if (rank == 0) {
        Datatype* matrix1 = initMatrix(size);
        Datatype* matrix2 = initMatrix(size);
        Datatype* parallel_output = initMatrix(size);
        Datatype* cuda_output = initMatrix(size);
        Datatype* serial_output = initMatrix(size);

        randomSquareMat(matrix1, size);
        randomSquareMat(matrix2, size);

        long long timein,timeout;
        multiply(matrix1, matrix2, parallel_output, size, 1);

        timein = clock_read();
        sequentialMultiply(matrix1, matrix2, serial_output, size);
        timeout = clock_read();

        printf("Serial Clockcycles:\t%lld\n", timeout-timein);

        timein = clock_read();
        master_routine(matrix1, matrix2, parallel_output, rank, world_size, size);
        timeout = clock_read();

        MPI_Barrier(MPI_COMM_WORLD);
        printf("MPI Clockcycles:\t%lld\n", timeout-timein);


        timein = clock_read();
        cuda_master_routine(matrix1, matrix2, cuda_output, rank, world_size, size);
        timeout = clock_read();

        printf("CUDA Clockcycles:\t%lld\n", timeout-timein);


        printf("Is the output matrix correct? %s\n", matrices_are_equal(parallel_output, serial_output, size) ? "yes" : "no" );

        //printMatrix(parallel_output, size);
        //printf("\n");
        //printMatrix(serial_output, size);

        freeMatrix(matrix1);
        freeMatrix(matrix2);
        freeMatrix(parallel_output);
        freeMatrix(serial_output);
    } else {
        worker_routine(rank, world_size, size);

        MPI_Barrier(MPI_COMM_WORLD);

        cuda_worker_routine(rank, world_size, size);
    }

    MPI_Finalize();

    return 0;
}
