#include "mathextension.h"

void mathExtension::matrixMultiplyScalar(double **m, int nrow, int ncol, double s) {

    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol; j++) {
            m[i][j] *= s;
        }
    }

}

template <typename T , size_t rows, size_t cols>
void mathExtension::matrixMultiplyMatrix(T (&array1)[rows][cols], T(&array2)[rows][cols], T(&array)[rows][cols])
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j) {
            array1[i][j]*array2[j][i];
        }
    }
}

template <typename T , size_t rows, size_t cols>
void mathExtension::matrixMultiplyVector(T (&array1)[rows][cols], T(&array2)[rows], T(&array)[rows])
{

}


void mathExtension::matrixMultiplyMatrix(double **m, int nrow_m, int ncol_m, double **n, int nrow_n, int ncol_n)
{

}


void mathExtension::matrixMultiplyVector(double **m, int nrow_m, int ncol_n, double *n, int nrow_n)
{

}
