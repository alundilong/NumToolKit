#ifndef MATHEXTENSION_H
#define MATHEXTENSION_H

namespace mathExtension {

void matrixMultiplyScalar(double **m, int nrow, int ncol, double s);

void matrixMultiplyMatrix(double **m, int nrow_m, int ncol_m, double **n, int nrow_n, int ncol_n);

void matrixMultiplyVector(double **m, int nrow_m, int ncol_n, double *n, int nrow_n);

// gauss point integration

}

#endif // MATHEXTENSION_H
