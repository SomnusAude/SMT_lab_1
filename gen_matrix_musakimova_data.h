#ifndef GEN_MATRIX_MUSTAKIMOVA_DATA_H
#define GEN_MATRIX_MUSTAKIMOVA_DATA_H

#include <stdlib.h>
#include <math.h>

void applyDifferentialTask(size_t n, double ***matrix, double **rhs, double val_start, double val_end) {
    *matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        (*matrix)[i] = (double *)malloc(n * sizeof(double));
    }

    *rhs = (double *)malloc(n * sizeof(double));

    double h = (1.0 / (n - 1.0));

    for (int i = 0; i < n; i++) {
        if (i == 0){
            (*matrix)[i][i] = 1.0;
            (*rhs)[i] = (double)val_start;
        }
        else if (i == n-1){
            (*matrix)[i][i] = 1.0; 
            (*rhs)[i] = (double)val_end;
        }
        else{
            (*matrix)[i][i+1] = (1.0 / (h * h)) - (7.0 / h);
            (*matrix)[i][i] = (-2.0 / (h * h)) + (10.0 / h) + 3.0;
            (*matrix)[i][i-1] = (1.0 / (h * h)) - (3.0 / h);
            //printf("i = %f\n", h*h*(2.0*i*h - 2.0));
            (*rhs)[i] = (2*i*h - 2);
        }
    }
};

double computeAnalyticExpression(int i, double h) {
    double x = i * h;
    double result = (2 * (-14 * exp(x) + 14 * exp(3 * x) + 8 * exp(3 + x) - 8 * exp(1 + 3 * x) - exp(1) * (1 + 3 * x) + exp(3) * (1 + 3 * x))) / (9 * exp(1) * (-1 + exp(2)));
    return result;
};

double compute_error(double (*analytical)(int, double), gsl_vector* solution, size_t n){
    double error;
    double h = (1.0 / (n - 1.0));

    for (int i = 0; i < n; i++){
        printf("analytic[%d] = %f\n", i, analytical(i, h));
        error = error + (gsl_vector_get(solution, i) - analytical(i, h))*(gsl_vector_get(solution, i) - analytical(i, h));
    }

    return sqrt(error);
}


double compute_inf_error(double (*analytical)(int, double), gsl_vector* solution, size_t n){
    double error;
    double h = (1.0 / (n - 1.0));
    double max_err = 0;
    for (int i = 0; i < n; i++){
      if (abs(gsl_vector_get(solution, i) - analytical(i, h))) > max_err{
            max_err = abs((gsl_vector_get(solution, i) - analytical(i, h)));
         }
    }
    return max_err;
}
#endif //GEN_MATRIX_MUSTAKIMOVA_DATA_H
