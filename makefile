all: main.c matrix.c matrix_mpi.c matrix.cu
	mpicc -O3 main.c -c -o main-mpi.o
	mpicc -O3 matrix.c -c -o matrix-mpi.o
	mpicc -O3 matrix_mpi.c -c -o matrix_mpi-mpi.o
	nvcc -O3 -arch=sm_70 matrix.cu -c -o matrix-cuda.o
	mpicc -O3 main-mpi.o matrix-mpi.o matrix_mpi-mpi.o matrix-cuda.o -o matrix-exe -L/usr/local/cuda-11.2/lib64/ -lcudadevrt -lcudart -lstdc++