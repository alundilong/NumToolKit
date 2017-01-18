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

/* Look up table for fast calculation of Legendre polynomial for n<1024 */
// source : https://pomax.github.io/bezierinfo/legendre-gauss.html

// 2 points
static const long double gaussQuadrature2PW[2][2] = {
    {1.0000000000000000,-0.5773502691896257},
    {1.0000000000000000,0.5773502691896257}
};

// 3 points
static const long double gaussQuadrature3PW[3][2] = {
    { 0.8888888888888888,0.0000000000000000},
    { 0.5555555555555556,-0.7745966692414834},
    {0.5555555555555556,0.7745966692414834}
};

// 4 points
static const long double gaussQuadrature4PW[4][2] = {
    { 0.6521451548625461,-0.3399810435848563},
    { 0.6521451548625461,0.3399810435848563},
    {0.3478548451374538,-0.8611363115940526},
    {0.3478548451374538,0.8611363115940526}
};

// 5 points
static const long double gaussQuadrature5PW[5][2] = {
    { 0.5688888888888889,0.0000000000000000},
    { 0.4786286704993665,-0.5384693101056831},
    {0.4786286704993665,0.5384693101056831},
    {0.2369268850561891,-0.9061798459386640},
    {0.2369268850561891,0.9061798459386640}
};

long double mathExtension::gaussP(int nSize, int p) {

    switch (nSize) {
    case 2: {
        return gaussQuadrature2PW[p][1];
        break;
    }
    case 3: {
        return gaussQuadrature3PW[p][1];
        break;
    }
    case 4: {
        return gaussQuadrature4PW[p][1];
        break;
    }
    case 5: {
        return gaussQuadrature5PW[p][1];
        break;
    }
    default:
    {
        std::cout << nSize << \
                     " point Gauss Quadrature table is not implemented!" \
                  << std::endl;
        return 0;
        break;
    }
    }

}


long double mathExtension::gaussW(int nSize, int p) {
    switch (nSize) {
    case 2: {
        return gaussQuadrature2PW[p][0];
        break;
    }
    case 3: {
        return gaussQuadrature3PW[p][0];
        break;
    }
    case 4: {
        return gaussQuadrature4PW[p][0];
        break;
    }
    case 5: {
        return gaussQuadrature5PW[p][0];
        break;
    }
    default:
    {
        std::cout << nSize << \
                     "-Point Gauss Quadrature table is not implemented!" \
                  << std::endl;
        return 0;
        break;
    }
    }
}
