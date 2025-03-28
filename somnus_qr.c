#include <stdio.h>
#include <stdlib.h>
#include <lapacke.h>
#include "gen_matrix_somnus_data.h"
#include <math.h>

int main()
{
    double **matrix;
    double *vector;
    size_t n = 160;

    applyDifferentialTask(n, &matrix, &vector, 0, 2);

    printf("Matrix:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("Rhs:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%.6f\n", vector[i]);
    }

    double *flat_matr = malloc(n * n * sizeof(double));
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            flat_matr[i * n + j] = matrix[i][j];
        }
    }

    int info;
    double *tau = malloc(n * sizeof(double));

    info = LAPACKE_dgeqrf(LAPACK_ROW_MAJOR, n, n, flat_matr, n, tau);
    if (info != 0)
    {
        printf("QR decomposition failed\n");
        return -1;
    }

    info = LAPACKE_dormqr(LAPACK_ROW_MAJOR, 'L', 'T', n, 1, n, flat_matr, n, tau, vector, 1);
    if (info != 0)
    {
        printf("Multiplication by Q^T failed\n");
        return -1;
    }

    info = LAPACKE_dtrtrs(LAPACK_ROW_MAJOR, 'U', 'N', 'N', n, 1, flat_matr, n, vector, 1);
    if (info != 0)
    {
        printf("Solving Rx = Q^Tb failed\n");
        return -1;
    }

    printf("Solution:\n");
    for (size_t i = 0; i < n; i++)
    {
        printf("%.6f\n", vector[i]);
    }

    double error = compute_error(computeAnalyticExpression, vector, n);
    printf("error = %f\n", error);

    for (size_t i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(flat_matr);
    free(tau);

    return 0;
}
