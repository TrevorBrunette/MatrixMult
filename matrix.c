#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "matrix.h"

bool matrices_are_equal(const Datatype* matrix1, const Datatype* matrix2, int size) {
    int size2 = size * size;
    for(int i = 0; i < size2; ++i){
        if(matrix1[i] != matrix2[i]){
            return false;
        }
    }
    return true;
}

Datatype* initMatrix(int size) {
    return calloc(size * size, sizeof(Datatype));
}

Datatype* initSlice(int length) {
    return calloc(length, sizeof(Datatype));
}


void freeMatrix(Datatype* matrix_ptr) {
    free(matrix_ptr);
}

void freeSlice(Datatype* slice_ptr) {
    free(slice_ptr);
}


void printMatrix(const Datatype* matrix, int size) {
    int size2 = size * size;
    for(int i = 0; i < size2; ++i){
        printf("%12.2lf ", matrix[i]);
        if(i % size == size - 1) printf("\n");
    }
    printf("\n");
}

void randomSquareMat(Datatype* matrix, int size) {
    int size2 = size*size;
    for(int i = 0; i < size2; ++i){
        matrix[i] = i;
    }
}

void sequentialMultiply(const Datatype* mat1, const Datatype* mat2, Datatype* mat3, int size) {
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            for(int k = 0; k < size; ++k){
                mat3[j + i * size] += mat1[k + i * size] * mat2[j + k * size];
            }
        }
    }
}

