// AI Generated Code.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int **allocate_matrix(int rows, int cols) {
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

void generate_random_matrix(int **matrix, int rows, int cols, int min_val, int max_val) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = min_val + rand() % (max_val - min_val + 1);
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

void alpha_beta_matrix_multiplication(int **A, int **B, int **C, int n) {
    int *sumArows = (int *)malloc(n * sizeof(int));
    int *sumBcols = (int *)malloc(n * sizeof(int));
    double *firstItemOfRowBarSumArows = (double *)malloc(n * sizeof(double));
    double *inverseOfEachSumArows = (double *)malloc(n * sizeof(double));
    double *inverseOfEachSumBcols = (double *)malloc(n * sizeof(double));
    double *first_item_sub_all = (double *)malloc((n - 1) * sizeof(double));

    for (int i = 0; i < n; i++) {
        sumArows[i] = 0;
        sumBcols[i] = 0;
        for (int j = 0; j < n; j++) {
            sumArows[i] += A[i][j];
            sumBcols[i] += B[j][i];
        }
        inverseOfEachSumArows[i] = (sumArows[i] != 0) ? 1.0 / sumArows[i] : 0;
        inverseOfEachSumBcols[i] = (sumBcols[i] != 0) ? 1.0 / sumBcols[i] : 0;
        firstItemOfRowBarSumArows[i] = (sumArows[i] != 0) ? (double)A[i][0] / sumArows[i] : 0;
    }

    for (int a_idx = 0; a_idx < n; a_idx++) {
        double sa_a_idx = sumArows[a_idx];
        double fiorbsa_a_idx = firstItemOfRowBarSumArows[a_idx];
        double ioesa_a_idx = inverseOfEachSumArows[a_idx];

        for (int i = 1; i < n; i++) {
            first_item_sub_all[i - 1] = A[a_idx][i] - A[a_idx][0];
        }

        for (int b_idx = 0; b_idx < n; b_idx++) {
            double beta = 0;
            for (int i = 0; i < n - 1; i++) {
                beta += B[i + 1][b_idx] * first_item_sub_all[i];
            }
            double alpha = fiorbsa_a_idx + (beta * ioesa_a_idx * inverseOfEachSumBcols[b_idx]);
            C[a_idx][b_idx] = (int)round(sa_a_idx * sumBcols[b_idx] * alpha);
        }
    }

    free(sumArows);
    free(sumBcols);
    free(firstItemOfRowBarSumArows);
    free(inverseOfEachSumArows);
    free(inverseOfEachSumBcols);
    free(first_item_sub_all);
}

int matrices_are_equal(int **A, int **B, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((int)(A[i][j]) != (int)(B[i][j])) {
                return 0;
            }
        }
    }
    return 1;
}

void run_benchmark(int n, int num_runs, FILE *csv_file) {
    int **A = allocate_matrix(n, n);
    int **B = allocate_matrix(n, n);
    int **C_std = allocate_matrix(n, n);
    int **C_ab2 = allocate_matrix(n, n);
    
    generate_random_matrix(A, n, n, -1000, 1000);
    generate_random_matrix(B, n, n, -1000, 1000);
    
    clock_t start, end;
    double time_std = 0.0, time_ab2 = 0.0;
    
    for (int run = 0; run < num_runs; run++) {
        start = clock();
        standard_matrix_multiplication(A, B, C_std, n);
        end = clock();
        time_std += ((double)(end - start)) / CLOCKS_PER_SEC;
    }
    time_std /= num_runs;
    
    
    for (int run = 0; run < num_runs; run++) {
        start = clock();
        alpha_beta_matrix_multiplication(A, B, C_ab2, n);
        end = clock();
        time_ab2 += ((double)(end - start)) / CLOCKS_PER_SEC;
    }
    time_ab2 /= num_runs;
    int ab2_correct = matrices_are_equal(C_std, C_ab2, n, n);
    
    printf("Matrix size %dx%d:\n", n, n);
    printf("  Standard: %.6f seconds\n", time_std);
    printf("  Alpha-Beta: %.6f seconds (%.2fx %s) - %s\n", 
           time_ab2, time_std / time_ab2, 
           time_std > time_ab2 ? "faster" : "slower",
           ab2_correct ? "correct" : "incorrect");
    printf("\n");
    
    if (csv_file) {
        fprintf(csv_file, "%d,%f,%f,%d\n", 
                n, time_std, time_ab2, ab2_correct);
    }
    
    // Free memory
    free_matrix(A, n);
    free_matrix(B, n);
    free_matrix(C_std, n);
    free_matrix(C_ab2, n);
}

int main() {
    srand(time(NULL));
    
    printf("Matrix Multiplication Algorithm Comparison\n");
    printf("=========================================\n\n");
    
    FILE *csv_file = fopen("matrix_comparison_results.csv", "w");
    if (!csv_file) {
        printf("Error creating CSV file\n");
        return 1;
    }
    
    fprintf(csv_file, "Size,Standard,AlphaBeta,Correctness\n");
    
    int sizes[] = {5, 10, 20, 50, 100, 200, 500, 1000, 1500, 2000, 2500, 3000};

    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);    
    int num_runs = 5;
    
    for (int i = 0; i < num_sizes; i++) {
        run_benchmark(sizes[i], num_runs, csv_file);
    }
    
    fclose(csv_file);
    
    printf("Benchmark complete! Results saved to 'matrix_comparison_results.csv'.\n");    
    return 0;
}

// matrix_comparison_results.csv
// Size,Standard,AlphaBeta,Correctness
// 5,0.000003,0.000005,1
// 10,0.000018,0.000019,1
// 20,0.000101,0.000119,1
// 50,0.000704,0.000340,1
// 100,0.002882,0.002431,1
// 200,0.022706,0.018150,1
// 500,0.383213,0.285080,1
// 1000,3.675808,2.708856,1
// 1500,14.398306,9.713360,1
// 2000,37.495369,26.228778,1
// 2500,74.025922,50.163575,1
// 3000,138.272042,104.693285,1
