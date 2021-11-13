#include <stdio.h>
#include <stdlib.h>
// #include <mpi.h>
#include "gaussian.h"

void gaussian_sequential(float **a, float *x, int n) {

    //Form Upper Triangular Matrix
    for (int v = 0; v < n - 1; v++) {
        for (int i = v + 1; i < n; i++) {
            float ratio = a[i][v] / a[v][v];
            for (int j = v; j < n + 1; j++) {
                a[i][j] -= (ratio * a[v][j]);
            }
        }
    }
    //Back Substitution
    float row_sum; 
    x[n - 1] = a[n - 1][n] / a[n - 1][n - 1];
    for (int i = n - 2; i >= 0; i--) {
        row_sum = 0;
        for(int j = i + 1; j < n; j++) {
            row_sum = row_sum + a[i][j] * x[j];
        }
        x[i] = (a[i][n] - row_sum) / a[i][i];
    }
}

void gaussian_parallel(float **a, float *x, int n) {

}
void gaussian_parallel_collective(float **a, float *x, int n);

// int main() {
//     int n_rows = 3;
//     int n_cols = 4;

//     float **a;
//     a = malloc(n_rows * sizeof *a);
//     a[0] = malloc(n_rows * n_cols * sizeof *a[0]);
//     for (int i = 1; i < n_rows; i++){
//         a[i] = a[i - 1] + n_cols;
//     }

//     for (int i = 0; i < n_rows; i++) {
//         for (int j = 0; j < n_cols; j++) {
//             a[i][j] = i + j + 1;
//         }
//     }
//     a[0][0] = 5;
//     a[1][2] = 2;
//     a[2][2] = 1;

//     float *x;
//     x = malloc(n_rows * sizeof *x);
//     print_matrix(a, n_rows);
//     printf("\n");

//     gaussian_sequential(a, x, n_rows);

//     print_matrix(a, n_rows);
//     for (int i = 0; i < n_rows; i++) {
//         printf("%f ", x[i]);
//     }
//     printf("\n");

//     return 0;
// }