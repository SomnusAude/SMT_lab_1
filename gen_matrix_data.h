#ifndef GEN_MATRIX_DATA_H
#define GEN_MATRIX_DATA_H

#include <stdlib.h>
#include <math.h>

void createDifferentialEquation(size_t n, double ***matrix, double **rhs, double a1, double a2, double a3, double b1, double b2, double val_start, double val_end) {
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
            (*matrix)[i][i+1] = 2.0 - 5.0 * h;
            (*matrix)[i][i] = -4.0 + 8.0 * h - 4.0 * h * h;
            (*matrix)[i][i-1] = 2.0 - 3.0 * h;
            (*rhs)[i] = 2*h*h*(-6.0*i*h-9.0);
        }
    }
}

double computeAnalyticExpression(int i, double h) {
    double x = i * h;

    double numerator = 3.0 * exp(-x) * (-exp(3.0) + exp(3.0 * x) - exp(x) * (1.0 + x) + exp(3.0 + x) * (1.0 + x));
    double denominator = -1.0 + exp(3.0);

    return numerator / denominator;
}

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
    double h = (1.0 / (n - 1.0));
    double max_err = 0.0;
    for (int i = 0; i < n; i++){
      if (sqrt((gsl_vector_get(solution, i)-analytical(i, h))*(gsl_vector_get(solution, i)-analytical(i, h))) > max_err){
            max_err = sqrt((gsl_vector_get(solution, i)-analytical(i, h))*(gsl_vector_get(solution, i)-analytical(i, h)));
         }
    }
    return max_err;
}

#endif //GEN_MATRIX_DATA_H
