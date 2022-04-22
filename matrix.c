#include "matrix.h"
#include <mpi.h>
#include <stdlib.h>

void randomSquareMat(Datatype** matrix, int dimension){
    for(int i = 0; i < dimension; ++i){
        for(int j = 0; j < dimension; ++j){
            matrix[i][j] = (2);
        }
    }
}

void sequentialMultiply(Datatype** mat1, Datatype** mat2, Datatype** mat3, int dimension){
    for(int i = 0; i < dimension; ++i){
        for(int j = 0; j<dimension; ++j){
            for(int k = 0; k<dimension; ++k){
                mat3[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

void parallelMultiply(Datatype** mat1, Datatype** mat2, Datatype** mat3, int dimension, int rank, int world_size){

    for(int i=0; i < dimension/rank; ++i){
        int row_index = i * rank;
        for(int j=0; j<dimension; ++j){
            for(int k=0; k<dimension; ++k){
                mat3[row_index][j] += mat1[row_index][k] * mat2[k][j];
            }
        }
    }
}

