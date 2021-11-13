#include <stdio.h>
#include <stdlib.h>
#include "gaussian.h"

void print_matrix(float **a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            printf("%f ", a[i][j]);
        }
        printf("\n");
    }
}

float **allocate_matrix(int n) {
    float **matrix;
    int n_rows = n;
    int n_cols = n + 1; // for the augmented matrix

    matrix = malloc(n_rows * sizeof *matrix);
    if ( !matrix ) {
        fprintf(stderr, "Error: malloc rows failed.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate space for the augmented matrix cols
    matrix[0] = malloc(n_rows * n_cols * sizeof *matrix[0]);
    if ( !matrix ) {
        fprintf(stderr, "Error: malloc cols failed.\n");
        free(matrix);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < n_rows; i++){
        matrix[i] = matrix[i - 1] + n_cols; //put it in 2D format (move cols)
    }
    return matrix;
}

void gaussian_sequential(float **a, float *x, int n) {
    // Form upper triangular matrix
    for (int v = 0; v < n - 1; v++) {
        for (int i = v + 1; i < n; i++) {
            float ratio = a[i][v] / a[v][v];
            for (int j = v; j < n + 1; j++) {
                a[i][j] -= (ratio * a[v][j]);
            }
        }
    }
    // Back substitution
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

void gaussian_parallel(float **a, float *x, int n, int comm_size, int comm_rank, MPI_Comm comm) {
    MPI_Status status;

    // Sync number of equations
    if (comm_rank == 0) {
        for (int proc = 1; proc < comm_size; proc++) {
            MPI_Send(&n, 1, MPI_INT, proc, 0, comm);
        }
    }
    if (comm_rank !=0) {
        MPI_Recv(&n, 1, MPI_INT, 0, 0, comm, &status);
    }

    // Sync processes allocation and input matrix a

    if (comm_rank == 0) {
        for (int proc = 1; proc < comm_size; proc++) {
            MPI_Send(a[0], n * (n + 1), MPI_FLOAT, proc, 0, comm); 
        }
    }
    if (comm_rank !=0) {
        a = allocate_matrix(n);
        MPI_Recv(a[0], n * (n + 1), MPI_FLOAT, 0, 0, comm, &status);
    }

    // Form upper triangular matrix, zeroing subsequent rows is distributed
    for (int v = 0; v < n - 1; v++) { // loop on pivots
        MPI_Barrier(comm);
        // int start = ((v + 1) / comm_size) * comm_size + comm_rank;
        for (int i = v + 1; i < n; i++) { // loop on subsequent rows
            if (i % comm_size == comm_rank) {
                float ratio = a[i][v] / a[v][v];
                for (int j = v; j < n + 1; j++) { // loop on the elements of each rows
                    a[i][j] -= (ratio * a[v][j]);
                }
            }
        }
        // Sync each pivot update
        if (comm_rank !=0) {
            for (int i = 0; i < n; i++) {
                if (i % comm_size == comm_rank) {
                    MPI_Send(a[i], n + 1, MPI_FLOAT, 0, 0, comm);
                }
            }
            MPI_Recv(a[0], n * (n + 1), MPI_FLOAT, 0, 0, comm, &status);
        }
        if (comm_rank ==0) {
            for (int i = 0; i < n; i++) {
                int proc = i % comm_size;
                if (proc){
                    MPI_Recv(a[i], n + 1, MPI_FLOAT, proc, 0, comm, &status);
                }
            }
            for (int proc = 1; proc < comm_size; proc++) {
                MPI_Send(a[0], n * (n + 1), MPI_FLOAT, proc, 0, comm); 
            }
        }
    }


    // Back substitution, calculating the sum for each row is distributed
    if (comm_rank ==0) {
        float row_sum;
        float sum;
        x[n - 1] = a[n - 1][n] / a[n - 1][n - 1];
        for (int i = n - 2; i >= 0; i--) {
            row_sum = 0;
            for(int proc = 1; proc < comm_size; proc++) {
                MPI_Send(&x[i+1], 1, MPI_FLOAT, proc, 0, comm); 
                MPI_Recv(&sum, 1, MPI_FLOAT, proc, 0, comm, &status);
                row_sum += sum;
            }
            for(int j = i + 1; j < n; j += comm_size) {
                row_sum = row_sum + a[i][j] * x[j];
            }
            x[i] = (a[i][n] - row_sum) / a[i][i];
        }
    }

    if (comm_rank !=0) {
        float sum; 
        for (int i = n - 2; i >= 0; i--) {
            sum = 0;
            MPI_Recv(&x[i+1], 1, MPI_FLOAT, 0, 0, comm, &status); 
            for(int j = i + 1 + comm_rank; j < n; j += comm_size) {
                sum = sum + a[i][j] * x[j];
            }
            MPI_Send(&sum, 1, MPI_FLOAT, 0, 0, comm);
        }
    }
}

void gaussian_parallel_collective(float **a, float *x, int n, int comm_size, int comm_rank) {

    if (comm_rank != 0) {
        a = allocate_matrix(n);
    }
    int *proc_allocation;
    proc_allocation = malloc(n * sizeof *proc_allocation);

    if (comm_rank == 0) {
        for (int row = 0; row < n; row++)
            proc_allocation[row] = row % comm_size;
    }
    MPI_Bcast(proc_allocation, n , MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(a[0], n * (n + 1), MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Form upper triangular matrix
    for (int v = 0; v < n - 1; v++) {
        for (int i = v + 1; i < n; i++) {
            if (proc_allocation[i] == comm_rank) {
                float ratio = a[i][v] / a[v][v];
                for (int j = v; j < n + 1; j++) {
                    a[i][j] -= (ratio * a[v][j]);
                    printf("%f ", a[i][j]);
                }
                printf("\n");

            }
        }

    }
    

}

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