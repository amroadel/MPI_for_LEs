#ifndef GAUSSIAN
#define GAUSSIAN

#include <mpi.h>

float **allocate_matrix(int n);
void gaussian_sequential(float ** a, float *x, int n);
void gaussian_parallel(float ** a, float *x, int n, int comm_size, int comm_rank, MPI_Comm comm);
void gaussian_parallel_collective(float **a, float *x, int n, int comm_size, int comm_rank, MPI_Comm comm);

#endif