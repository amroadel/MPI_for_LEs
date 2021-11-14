#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h> 
#include "gaussian.h"

float **read_matrix_from_file(char *file_name, int *n_equations) {

    float **matrix;
    FILE * input = fopen(file_name, "r");
    if ( !input ) {
        fprintf(stderr, "Error: Unable to open input file.\n");
        exit(EXIT_FAILURE);
    }

    // Read the number of equations
    fscanf(input, "%d", n_equations);
    
    int n_rows = *n_equations;
    int n_cols = *n_equations + 1; // for the augmented matrix

    // Allocate space for the augmented matrix rows
    matrix = allocate_matrix(*n_equations);

    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            fscanf(input, "%f", &matrix[i][j]); 
        }
    }
    fclose(input);
    return matrix;

}

// void print_matrix(float **a, int n) {
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n + 1; j++) {
//             printf("%f ", a[i][j]);
//         }
//         printf("\n");
//     }
// }

int main(int argc, char **argv) {

    int n_equations; 
    float **matrix;
    float *x;
    
     
    

    int comm_size, comm_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    if (comm_rank == 0) {
 
        matrix = read_matrix_from_file(argv[1], &n_equations);
        x = malloc(n_equations * sizeof *x);
  
    }


    
    gaussian_parallel_collective(matrix, x, n_equations, comm_size, comm_rank, MPI_COMM_WORLD);

    if (comm_rank == 0) {
        for (int i = 0; i < n_equations; i++) {
            printf("x%d = %f \n", i, x[i]);
        }
    }

    // free(matrix[0]);
    // free(matrix);
    MPI_Finalize();
    return 0;
}