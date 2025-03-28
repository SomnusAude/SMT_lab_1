#ifndef GEN_MATRIX_SOMNUS_DATA_H
#define GEN_MATRIX_SOMNUS_DATA_H

#include <stdlib.h>
#include <math.h>

void applyDifferentialTask(size_t n, double ***matrix, double **rhs, double val_start, double val_end)
{
    *matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        (*matrix)[i] = (double *)malloc(n * sizeof(double));
    }

    *rhs = (double *)malloc(n * sizeof(double));

    double h = (1.0 / (n - 1.0));

    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            (*matrix)[i][i] = 1.0;
            (*rhs)[i] = (double)val_start;
        }
        else if (i == n - 1)
        {
            (*matrix)[i][i] = 1.0;
            (*rhs)[i] = (double)val_end;
        }
        else
        {
            (*matrix)[i][i + 1] = (6.0 / h) + 14.0;
            (*matrix)[i][i] = (-12.0 / h) + 16.0;
            (*matrix)[i][i - 1] = (6.0 / h) - 2.0;
            (*rhs)[i] = -24.0 * x_i * h;
        }
    }
};

double computeAnalyticExpression(int i, double h)
{
    return 2.0 * i * h;
};

double compute_error(double (*analytical)(int, double), gsl_vector *solution, size_t n)
{
    double error;
    double h = (1.0 / (n - 1.0));

    for (int i = 0; i < n; i++)
    {
        printf("analytic[%d] = %f\n", i, analytical(i, h));
        error = error + (gsl_vector_get(solution, i) - analytical(i, h)) * (gsl_vector_get(solution, i) - analytical(i, h));
    }

    return sqrt(error);
}

double compute_inf_error(double (*analytical)(int, double), gsl_vector *solution, size_t n)
{
    double h = (1.0 / (n - 1.0));
    double max_err = 0.0;
    for (int i = 0; i < n; i++)
    {
        if (sqrt((gsl_vector_get(solution, i) - analytical(i, h)) * (gsl_vector_get(solution, i) - analytical(i, h))) > max_err)
        {
            max_err = sqrt((gsl_vector_get(solution, i) - analytical(i, h)) * (gsl_vector_get(solution, i) - analytical(i, h)));
        }
    }
    return max_err;
}

#endif // GEN_MATRIX_SOMNUS_DATA_H
