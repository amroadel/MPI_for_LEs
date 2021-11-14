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
    
    double time_spent = 0.0;
    int n_equations; 
    float **matrix;
    float *x;
    

    matrix = read_matrix_from_file(argv[1], &n_equations);
    x = malloc(n_equations * sizeof *x);


    clock_t begin = clock();
    gaussian_sequential(matrix, x, n_equations);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    for (int i = 0; i < n_equations; i++) 
        printf("x%d = %f \n", i, x[i]);

    printf("Sequential time is %f seconds\n", time_spent);
    free(matrix[0]);
    free(matrix);

    return 0;
}