#ifndef GAUSSIAN
#define GAUSSIAN

void gaussian_sequential(float ** a, float *x, int n);
void gaussian_parallel(float ** a, float *x, int n);
void gaussian_parallel_collective(float ** a, float *x, int n);

#endif