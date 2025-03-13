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

void precompute(int **A, int **B, int n, int *sumArows, int *sumBcols,
                double *firstItemOfRowBarSumArows, double *inverseOfEachSumArows, double *inverseOfEachSumBcols,
                int **first_item_sub_all) {
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
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j++) {
            first_item_sub_all[i][j - 1] = A[i][j] - A[i][0];
        }
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

void benchmark(int n, FILE *csv) {
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

  // Precompute phase
  start = clock();
  precompute(A, B, n, sumArows, sumBcols, firstItemOfRowBarSumArows, inverseOfEachSumArows, inverseOfEachSumBcols, first_item_sub_all);
  end = clock();
  precompute_time = ((double)(end - start)) / CLOCKS_PER_SEC;
  int iteration_limit = 5;
  // Run standard and optimized multiplication 5 times each
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

  // Compute averages
  standard_time /= iteration_limit;
  optimized_time /= iteration_limit;

  // Check correctness
  int correct = 1;
  for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
          if (C1[i][j] != C2[i][j]) {
              correct = 0;
              break;
          }
      }
  }

  double speedup = standard_time / optimized_time;

  // Write results to CSV
  fprintf(csv, "%d,%.6f,%.6f,%d,%.6f\n", n, standard_time, optimized_time, correct ? 1 : 0, speedup);

  // Free allocated memory
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
}


int main() {
    srand(time(NULL));
    FILE *csv = fopen("benchmark_results.csv", "w");
    fprintf(csv, "Size,Standard,AlphaBeta,Correctness,Speedup\n");
    int sizes[] = {100, 200, 300, 400, 500, 1000, 1500, 2000, 2500, 3000};
    for (int i = 0; i < 10; i++) {
        benchmark(sizes[i], csv);
    }
    fclose(csv);
    return 0;
}
