cmake_minimum_required(VERSION 3.20)
project(final CUDA)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER mpicc)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -lm")

set(SOURCE_FILES main.c matrix.c matrix_mpi.c matrix.cu)

add_executable(final ${SOURCE_FILES})
target_link_libraries(final ${MPI_C_LIBRARIES} m)