#ifndef MATHEXTENSION_H
#define MATHEXTENSION_H

#include <iostream>

namespace mathExtension {

void matrixMultiplyScalar(double **m, int nrow, int ncol, double s);// m will be modified

void matrixMultiplyMatrix(double **m, int nrow_m, int ncol_m, double **n, int nrow_n, int ncol_n);

void matrixMultiplyVector(double **m, int nrow_m, int ncol_n, double *n, int nrow_n);

template <typename T , size_t rows, size_t cols>
void matrixMultiplyMatrix(T (&array1)[rows][cols], T(&array2)[rows][cols], T(&array)[rows][cols]);

template <typename T , size_t rows, size_t cols>
void matrixMultiplyVector(T (&array1)[rows][cols], T(&array2)[rows], T(&array)[rows]);

// gauss point and weight look-up table

long double gaussP(int nSize, int p);

long double gaussW(int nSize, int p);

}

#endif // MATHEXTENSION_H
