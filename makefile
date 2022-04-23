all: main.c matrix.c matrix_mpi.c matrix.cu
	mpicc -O3 main.c -c -o build/main-mpi.o
	mpicc -O3 matrix.c -c -o build/matrix-mpi.o
	mpicc -O3 matrix_mpi.c -c -o build/matrix_mpi-mpi.o
	nvcc -O3 -arch=sm_70 matrix.cu -c -o build/matrix-cuda.o
	mpicc -O3 build/main-mpi.o build/matrix-mpi.o build/matrix_mpi-mpi.o build/matrix-cuda.o -o matrix-exe -L/usr/local/cuda-11.2/lib64/ -lcudadevrt -lcudart -lstdc++
