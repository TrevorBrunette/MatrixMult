#include "main.h"

#define MASTER      0  // taskid of first task
#define TRIAL_COUNT 6
#define CUDA        2
#define MPI         1
#define SERIAL      0

//enables math checks of output matrices
#define TESTING 0

int main(int argc, char** argv) {

    int rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (argc != 2){
        printf("Missing argument for test style. Exiting...\n");
        MPI_Abort(MPI_COMM_WORLD, 2);
        exit(2);
    }

    int mode = atoi(argv[1]);

    if (mode != SERIAL && world_size < 2 ) {
        printf("Need at least two MPI tasks. Exiting.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }

    int num_sizes = 8;
    int sizes[] = {10, 100, 300, 500, 800, 1000, 2000, 3000};
    /* int num_sizes = 4; */
    /* int sizes[] = {10, 100, 300, 500}; */
    /* int num_sizes = 1; */
    /* int sizes[] = {4}; */

    if (rank == 0) {
        /* Datatype* parallel_output = initMatrix(size); */
        /* Datatype* cuda_output = initMatrix(size); */

        long long timein,timeout;

        for(int t = 0; t < num_sizes; ++t){
            int size = sizes[t];

            Datatype* matrix1 = initMatrix(size);
            Datatype* matrix2 = initMatrix(size);

            switch (mode) {
                case CUDA:
                    printf("CUDA Clockcycles| Size: %d, Ranks: %d\n", size, world_size);
                    break;
                case MPI:
                    printf("MPI Clockcycles| Size: %d, Ranks: %d\n", size, world_size);
                    break;

                case SERIAL:
                    printf("SERIAL Clockcycles| Size: %d, Ranks: %d\n", size, world_size);
                    break;
            }

            for(int i = 0; i < TRIAL_COUNT; ++i){
                Datatype* output = initMatrix(size);
                Datatype* serial_output = initMatrix(size);
                randomSquareMat(matrix1, size);
                randomSquareMat(matrix2, size);
                if(TESTING) sequentialMultiply(matrix1, matrix2, serial_output, size);
                switch (mode) {
                    case CUDA:
                        timein = clock_read();
                        cuda_master_routine(matrix1, matrix2, output, rank, world_size, size);
                        timeout = clock_read();
                        break;
                    case MPI:
                        timein = clock_read();
                        master_routine(matrix1, matrix2, output, rank, world_size, size);
                        timeout = clock_read();
                        break;

                    case SERIAL:
                        timein = clock_read();
                        sequentialMultiply(matrix1, matrix2, output, size);
                        timeout = clock_read();
                        break;
                }

                if(TESTING) assert(matrices_are_equal(output, serial_output, size));

                printf("%lld\n", timeout-timein);

                MPI_Barrier(MPI_COMM_WORLD);
                freeMatrix(serial_output);
                freeMatrix(output);
            }
            freeMatrix(matrix1);
            freeMatrix(matrix2);
        }
        //printMatrix(parallel_output, size);
        //printf("\n");
        //printMatrix(serial_output, size);

        /* freeMatrix(parallel_output); */
        /* freeMatrix(serial_output); */
    } else {
        for(int t = 0; t < num_sizes; ++t){
            int size = sizes[t];
            for(int i = 0; i < TRIAL_COUNT; ++i){
                switch (mode) {
                    case CUDA:
                        cuda_worker_routine(rank, world_size, size);
                        break;
                    case MPI:
                        worker_routine(rank, world_size, size);
                        break;

                    case SERIAL:
                        break;
                }
                MPI_Barrier(MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();

    return 0;
}
