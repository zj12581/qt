#include <stdio.h>
#include <stdlib.h>
#include <Fit.h>
// Function to fit a polynomial to given points using least squares
void polynomial_fit(int n, double x[], double y[], int degree, double* coeff) {
    // Create matrix elements dynamically
    double** X = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        X[i] = (double*)malloc((degree + 1) * sizeof(double));
    }

    double** XT = (double**)malloc((degree + 1) * sizeof(double*));
    for (int i = 0; i < degree + 1; i++) {
        XT[i] = (double*)malloc(n * sizeof(double));
    }

    double** XT_X = (double**)malloc((degree + 1) * sizeof(double*));
    for (int i = 0; i < degree + 1; i++) {
        XT_X[i] = (double*)malloc((degree + 1) * sizeof(double));
    }

    double* XT_Y = (double*)malloc((degree + 1) * sizeof(double));
    double* result = (double*)malloc((degree + 1) * sizeof(double));

    // Initialize matrices to zero
    for (int i = 0; i < degree + 1; i++) {
        XT_Y[i] = 0;
        result[i] = 0;
        for (int j = 0; j < degree + 1; j++) {
            XT_X[i][j] = 0;
        }
    }

    // Create the X matrix
    for (int i = 0; i < n; i++) {
        X[i][0] = 1;
        for (int j = 1; j < degree + 1; j++) {
            X[i][j] = X[i][j - 1] * x[i];
        }
    }

    // Transpose of X matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < degree + 1; j++) {
            XT[j][i] = X[i][j];
        }
    }

    // Calculate XT * X
    for (int i = 0; i < degree + 1; i++) {
        for (int j = 0; j < degree + 1; j++) {
            for (int k = 0; k < n; k++) {
                XT_X[i][j] += XT[i][k] * X[k][j];
            }
        }
    }

    // Calculate XT * Y
    for (int i = 0; i < degree + 1; i++) {
        for (int j = 0; j < n; j++) {
            XT_Y[i] += XT[i][j] * y[j];
        }
    }

    // Solve the equation XT_X * coeff = XT_Y
    // This can be done using any method to solve a system of linear equations

    // Store the coefficients in the array pointed to by coeff
    // Here, we are just copying the values for the sake of demonstration
    for (int i = 0; i < degree + 1; i++) {
        coeff[i] = XT_Y[i];
    }

    // Free dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(X[i]);
    }
    free(X);

    for (int i = 0; i < degree + 1; i++) {
        free(XT[i]);
    }
    free(XT);

    for (int i = 0; i < degree + 1; i++) {
        free(XT_X[i]);
    }
    free(XT_X);

    free(XT_Y);
    free(result);
}
