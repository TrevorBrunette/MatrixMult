#ifndef FINAL_MATRIX_H
#define FINAL_MATRIX_H

#include <stdbool.h>

typedef double Datatype;

Datatype* initMatrix(int size);

Datatype* initSlice(int length);

void freeMatrix(Datatype* matrix_ptr);

void freeSlice(Datatype* slice_ptr);

bool matrices_are_equal(const Datatype* matrix1, const Datatype* matrix2, int size);

void printMatrix(const Datatype* matrix, int size);

void randomSquareMat(Datatype* matrix, int size);

void sequentialMultiply(const Datatype* mat1, const Datatype* mat2, Datatype* mat3, int size);

#endif //FINAL_MATRIX_H
