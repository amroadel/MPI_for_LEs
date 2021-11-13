#ifndef GAUSSIAN
#define GAUSSIAN

void gaussian_sequential(float ** a, int n);
void gaussian_parallel(float ** a, int n);
void gaussian_parallel_collective(float ** a, int n);

#endif