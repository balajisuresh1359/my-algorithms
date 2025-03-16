#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int** allocate_matrix(int rows, int cols) {
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void initialize_matrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void standard_matrix_multiplication(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void precompute_for_left_matrix(int **A, int n, int *sumArows, double *firstItemOfRowBarSumArows, double *inverseOfEachSumArows, int **first_item_sub_all) {
    for (int i = 0; i < n; i++) {
        sumArows[i] = 0;
        for (int j = 0; j < n; j++) {
            sumArows[i] += A[i][j];
        }
        inverseOfEachSumArows[i] = (sumArows[i] != 0) ? 1.0 / sumArows[i] : 0;
        firstItemOfRowBarSumArows[i] = (sumArows[i] != 0) ? (double)A[i][0] / sumArows[i] : 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j++) {
            first_item_sub_all[i][j - 1] = A[i][j] - A[i][0];
        }
    }
}

void precompute_for_right_matrix(int **B, int n, int *sumBcols, double *inverseOfEachSumBcols) {
    for (int i = 0; i < n; i++) {
        sumBcols[i] = 0;
        for (int j = 0; j < n; j++) {
            sumBcols[i] += B[j][i];
        }
        inverseOfEachSumBcols[i] = (sumBcols[i] != 0) ? 1.0 / sumBcols[i] : 0;
    }
}

void alpha_beta_matrix_multiplication(int **A, int **B, int **C, int n,
                                      int *sumArows, int *sumBcols,
                                      double *firstItemOfRowBarSumArows,
                                      double *inverseOfEachSumArows, double *inverseOfEachSumBcols,
                                      int **first_item_sub_all) {
    for (int a_idx = 0; a_idx < n; a_idx++) {
        double sa_a_idx = sumArows[a_idx];
        double fiorbsa_a_idx = firstItemOfRowBarSumArows[a_idx];
        double ioesa_a_idx = inverseOfEachSumArows[a_idx];
        for (int b_idx = 0; b_idx < n; b_idx++) {
            double beta = 0;
            for (int i = 1; i < n; i++) {
                beta += B[i][b_idx] * first_item_sub_all[a_idx][i - 1];
            }
            double alpha = fiorbsa_a_idx + (beta * ioesa_a_idx * inverseOfEachSumBcols[b_idx]);
            C[a_idx][b_idx] = (int)round(sa_a_idx * sumBcols[b_idx] * alpha);
        }
    }
}

int validate_output(int **C1, int **C2, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (abs(C1[i][j] - C2[i][j]) > 1) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    int n = 100;
    srand(time(NULL));
    int **A = allocate_matrix(n, n);
    int **B = allocate_matrix(n, n);
    int **C1 = allocate_matrix(n, n);
    int **C2 = allocate_matrix(n, n);
    int *sumArows = (int *)malloc(n * sizeof(int));
    int *sumBcols = (int *)malloc(n * sizeof(int));
    double *firstItemOfRowBarSumArows = (double *)malloc(n * sizeof(double));
    double *inverseOfEachSumArows = (double *)malloc(n * sizeof(double));
    double *inverseOfEachSumBcols = (double *)malloc(n * sizeof(double));
    int **first_item_sub_all = allocate_matrix(n, n - 1);

    initialize_matrix(A, n);
    initialize_matrix(B, n);

    clock_t start, end;
    double precompute_time, standard_time = 0, optimized_time = 0;

    start = clock();
    precompute_for_left_matrix(A, n, sumArows, firstItemOfRowBarSumArows, inverseOfEachSumArows, first_item_sub_all);
    precompute_for_right_matrix(B, n, sumBcols, inverseOfEachSumBcols);
    end = clock();
    precompute_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    int iteration_limit = 5;
    for (int i = 0; i < iteration_limit; i++) {
        start = clock();
        standard_matrix_multiplication(A, B, C1, n);
        end = clock();
        standard_time += ((double)(end - start)) / CLOCKS_PER_SEC;

        start = clock();
        alpha_beta_matrix_multiplication(A, B, C2, n, sumArows, sumBcols, firstItemOfRowBarSumArows, inverseOfEachSumArows, inverseOfEachSumBcols, first_item_sub_all);
        end = clock();
        optimized_time += ((double)(end - start)) / CLOCKS_PER_SEC;
    }
    optimized_time += precompute_time;
    standard_time /= iteration_limit;
    optimized_time /= iteration_limit;

    const char *correct = validate_output(C1, C2, n) ? "Yes" : "No";
    double speedup = standard_time / optimized_time;
    
    printf("Dimension: %d * %d \nStandard 3 loop time taken: %.6f\nAlpha beta time taken: %.6f\nIs this correct? %s\nSpeed-up: %.6f\n",
            n,n, standard_time, optimized_time, correct, speedup);
    


    free_matrix(A, n);
    free_matrix(B, n);
    free_matrix(C1, n);
    free_matrix(C2, n);
    free_matrix(first_item_sub_all, n);
    free(sumArows);
    free(sumBcols);
    free(firstItemOfRowBarSumArows);
    free(inverseOfEachSumArows);
    free(inverseOfEachSumBcols);
    return 0;
}
