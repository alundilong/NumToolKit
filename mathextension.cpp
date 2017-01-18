#include "mathextension.h"

// Matrix x scalar
// matrix changed
void mathExtension::matrixMultiplyScalar(double **m, int nrow, int ncol, double s) {

    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol; j++) {
            m[i][j] *= s;
        }
    }

}


//void mathExtension::matrixMultiplyMatrix(double **m, int nrow_m, int ncol_m, double **n, int nrow_n, int ncol_n)
//{

//}


//void mathExtension::matrixMultiplyVector(double **m, int nrow_m, int ncol_n, double *n, int nrow_n)
//{

//}

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


mathExtension::Matrix::Matrix()
    :
      nrow_(1),
      ncol_(1),
      data_(NULL)
{
    data_ = new double *[nrow()];
    for (int i = 0; i < nrow(); i++) {
        data_[i] = new double[ncol()];
    }
    zeroize();
}

mathExtension::Matrix::Matrix(int nr, int nc)
    :
      nrow_(nr),
      ncol_(nc),
      data_(NULL)
{
    data_ = new double *[nrow()];
    for (int i = 0; i < nrow(); i++) {
        data_[i] = new double[ncol()];
    }
    zeroize();
}

mathExtension::Matrix::Matrix(int nr, int nc, double ** array)
    :
    nrow_(nr),
    ncol_(nc),
    data_(NULL)
{
    data_ = new double *[nrow()];
    for (int i = 0; i < nrow(); i++) {
        data_[i] = new double[ncol()];
    }

    for (int i = 0; i < nrow(); i++) {
        for (int j = 0; j < ncol(); j++) {
            data_[i][j] = array[i][j];
        }
    }
}

mathExtension::Matrix::~Matrix()
{
    for (int i = 0; i < nrow(); i ++){
        delete [] data_[i];
    }
    delete [] data_;
}

void mathExtension::Matrix::zeroize()
{
    for (int i = 0; i < nrow(); i++) {
        for (int j = 0; j < ncol(); j++) {
            data_[i][j] = 0.0;
        }
    }
}

mathExtension::Matrix mathExtension::Matrix::transpose()
{
    Matrix m(ncol(),nrow());

    for (int i = 0; i < nrow(); i++) {
        for (int j = 0; j < ncol(); j++) {
            m[j][i] = (*this)[i][j];
        }
    }
    return m;
}

void mathExtension::Matrix::operator=(const mathExtension::Matrix &m) const
{
    if((*this) == m) {
        for (int i = 0; i < nrow(); i++) {
            for (int j = 0; j < ncol(); j++) {
                (*this)[i][j] = m[i][j];
            }
        }
    }
}

mathExtension::Matrix mathExtension::Matrix::operator+(const mathExtension::Matrix &m) const
{
    if((*this) == m) {
        Matrix mm(nrow(), ncol());
        for (int i = 0; i < nrow(); i++) {
            for (int j = 0; j < ncol(); j++) {
                mm[i][j] = (*this)[i][j] + m[i][j];
            }
        }
        return mm;
    } else {
        std::cout << " Error: Matrix size is NOT consisent!!!!" << std::endl;
//        return;
    }
}

mathExtension::Matrix mathExtension::Matrix::operator-(const mathExtension::Matrix &m) const
{
    if((*this) == m) {
        Matrix mm(nrow(), ncol());
        for (int i = 0; i < nrow(); i++) {
            for (int j = 0; j < ncol(); j++) {
                mm[i][j] = (*this)[i][j] - m[i][j];
            }
        }
        return mm;
    } else {
        std::cout << " Error: Matrix size is NOT consisent!!!!" << std::endl;
//        return;
    }
}

mathExtension::Matrix mathExtension::Matrix::operator*(const mathExtension::Matrix &m) const
{
    if((*this).ncol() == m.nrow()) {
        Matrix mm(nrow(), ncol());
        for (int i = 0; i < nrow(); i++) {
            for (int j = 0; j < ncol(); j++) {
                for (int k = 0; k < nrow(); k++) {
                    mm[i][j] += (*this)[i][k]*m[k][j];
                }
            }
        }
        return mm;
    } else {
        std::cout << " Error: Matrix size is NOT consisent!!!!" << std::endl;
//        return;
    }
}

bool mathExtension::Matrix::operator==(const mathExtension::Matrix &m) const
{
    return ((*this).ncol() == m.ncol() && ((*this).nrow() == m.nrow()));
}

bool mathExtension::Matrix::operator!=(const mathExtension::Matrix &m) const
{
    return (!(*this == m));
}

double *mathExtension::Matrix::operator[](const int i) const
{
    return (*this).data_[i];
}
