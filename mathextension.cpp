#include "mathextension.h"

void mathExtension::matrixMultiplyScalar(double **m, int nrow, int ncol, double s) {

    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol; j++) {
            m[i][j] *= s;
        }
    }

}
