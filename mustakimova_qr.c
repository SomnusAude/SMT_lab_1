#include <stdio.h>
#include <gsl/gsl_linalg.h>
#include "gen_matrix_musakimova_data.h"
#include <math.h>

int main()
{
    double **matrix;
    double *vector;
    // Количество точек
    size_t n = 80;
    //Постановка разностной задачи в матрицу
    applyDifferentialTask(n, &matrix, &vector, 2, 4);

    printf("Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("Rhs:\n");
    for (int i = 0; i < n; i++) {
        printf("%.6f\n", vector[i]);
    }
    // Записываем полученную матрицу разностной схемы в вектор
    double *flat_matr = malloc(n * n * sizeof(double));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            flat_matr[i * n + j] = matrix[i][j];
        }
    }
    // Создаем gsl экземпляры задачи
    gsl_matrix_view A = gsl_matrix_view_array(flat_matr, n, n);
    gsl_vector_view b = gsl_vector_view_array(vector, n);
    // Выделение памяти для промежуточного вида вектора и Q,R матриц
    gsl_matrix *Q = gsl_matrix_alloc(n, n);
    gsl_matrix *R = gsl_matrix_alloc(n, n);
    gsl_vector *tau = gsl_vector_alloc(n);
    //Применяем разложение и переопределяем значения Q,R
    gsl_linalg_QR_decomp(&A.matrix, tau);
    gsl_linalg_QR_unpack(&A.matrix, tau, Q, R);
    //Транспонируем матрицу Q, получаем вид Rx = Q.T*b = qtb
    gsl_vector *Qtb = gsl_vector_alloc(n);
    gsl_blas_dgemv(CblasTrans, 1.0, Q, &b.vector, 0.0, Qtb);
    //Находим решение
    gsl_vector *x = gsl_vector_alloc(n);
    gsl_linalg_R_solve(R, Qtb, x);

    double error = compute_error(computeAnalyticExpression, x, n);
    printf("error = %f\n", error);

    for (size_t i = 0; i < n; i++) {
        free(matrix[i]);
    }
    // свобождение памяти
    free(matrix);
    free(vector);
    free(flat_matr);

    gsl_vector_free(x);

    return 0;
}
