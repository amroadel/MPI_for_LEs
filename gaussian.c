#include <stdio.h>
#include <stdlib.h>
#include "gaussian.h"

void gaussian_sequential(float **a, int n) {
    for (int v = 0; v < n - 1; v++) {
        for (int i = v + 1; i < n; i++) {
            float ratio = a[i][v] / a[v][v];
            for (int j = v; j < n + 1; j++) {
                a[i][j] -= (ratio * a[v][j]);
            }
        }
    }
}

void gaussian_parallel(float **a, int n);
void gaussian_parallel_collective(float **a, int n);

void print_matrix(float **a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            printf("%f ", a[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int n_rows = 3;
    int n_cols = 4;

    float **a;
    a = malloc(n_rows * sizeof *a);
    a[0] = malloc(n_rows * n_cols * sizeof *a[0]);
    for (int i = 1; i < n_rows; i++){
        a[i] = a[i - 1] + n_cols;
    }

    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            a[i][j] = i + j + 1;
        }
    }
    a[0][0] = 5;
    a[1][2] = 2;
    a[2][2] = 1;

    print_matrix(a, n_rows);
    printf("\n");

    gaussian_sequential(a, n_rows);

    print_matrix(a, n_rows);

    return 0;
}