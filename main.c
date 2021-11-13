#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    int i, j; 
    for (i = 1; i < n_rows; i++){
        matrix[i] = matrix[i - 1] + n_cols; //put it in 2D format (move cols)
    }

    for (i = 0; i < n_rows; i++) {
        for (j = 0; j < n_cols; j++) {
            fscanf(input, "%f", &matrix[i][j]); 
        }
    }
    fclose(input);
    return matrix;

}

void print_matrix(float **a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            printf("%f ", a[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {

    int n_equations; 
    float **matrix;
    float *x;

    x = malloc(n_equations * sizeof *x);
    matrix = read_matrix_from_file(argv[1], &n_equations);
    
    gaussian_sequential(matrix, x, n_equations);

    // print_matrix(matrix, n_equations);
    for (int i = 0; i < n_equations; i++) {
        printf("x%d = %f \n", i, x[i]);
    }

    free(matrix[0]);
    free(matrix);
    
    return 0;
}