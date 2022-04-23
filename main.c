#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "main.h"
#include "clockcycle.h"
#include "matrix.h"
#include <time.h>

#define MASTER 0   // taskid of first task

int main(int argc, char** argv) {
    const int size = 200;

    int rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

//    for(int i = 0; i < size; ++i){
//        for(int j = 0; j < size; ++j){
//            matrix1[i][j] = i+2*j;
//            matrix2[i][j] = 3*i+j;
//        }
//    }

//    matrix = malloc(size * sizeof(Datatype*));
//    for(int i = 0; i < size; ++i){
//        matrix[i] = malloc(size * sizeof (Datatype));
//    }

    const int size = 5;
    Datatype** matrix1 = malloc(size * sizeof(Datatype*));
    Datatype** matrix2 = malloc(size * sizeof(Datatype*));
    Datatype** matrix3 = malloc(size * sizeof(Datatype*));
    for(int i = 0; i < size; ++i){
        matrix1[i] = malloc(size * sizeof(Datatype));
        matrix2[i] = malloc(size * sizeof(Datatype));
        matrix3[i] = malloc(size * sizeof(Datatype));
    }

    randomSquareMat(matrix1, size);
    randomSquareMat(matrix2, size);

    if (rank == 0) {
        sequentialMultiply(matrix1, matrix2, matrix3, size);

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                printf("%12.2lf ", matrix3[i][j]);
            }
            printf("\n");
        }
    }

    for(int i = 0; i < size; ++i){
        free(matrix1[i]);
        free(matrix2[i]);
        free(matrix3[i]);
    }
    free(matrix1);
    free(matrix2);
    free(matrix3);

    MPI_Finalize();

    return 0;
}
