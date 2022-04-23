#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "clockcycle.h"
#include "matrix.h"
#include "matrix_mpi.h"

#ifndef RT_MAIN_H
#define RT_MAIN_H

extern void multiply(Datatype* matrix1, Datatype* matrix2, Datatype* matrix3, int size, int quantity);

#endif //RT_MAIN_H
