/* -*- c++ -*- ----------------------------------------------------------
    NumToolKit - Numerical Simulation ToolKit
    Yijin Mao, ymao.mu@gmail.com

    This file is part of NumToolKit.

    NumToolKit is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NumToolKit is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NumToolKit.  If not, see <http://www.gnu.org/licenses/>.

    This software is distributed under
    the GNU General Public License.

    See the README file in the top-level NumToolKit directory.
------------------------------------------------------------------------- */

#include "MathExtension.h"

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
    if(data_!= NULL) {

        for (int i = 0; i < nrow(); i ++){
            delete [] data_[i];
        }
        delete [] data_;
    }
}

void mathExtension::Matrix::set(const int i, const Vector &v)
{
    for (int j = 0; j < v.nrow(); j++) {
        data_[i][j] = v[j];
    }
}

void mathExtension::Matrix::setColValues(\
        const int iCol, \
        const mathExtension::pos &Cols, \
        const mathExtension::Vector &v)
{
    const int start = Cols.start;
    const int step = Cols.step;
    const int end = Cols.end;
    for (int c = start; c < end; c = c+step ) {
        data_[iCol][c-1] = v[c-1];
    }
}

void mathExtension::Matrix::setSubMatrix(\
        const mathExtension::pos &Rows, \
        const mathExtension::pos &Cols, \
        const mathExtension::Matrix &subM)
{
    const int rStart = Rows.start;
    const int rStep =  Rows.step;
    const int rEnd = Rows.end;

    const int cStart = Cols.start;
    const int cStep =  Cols.step;
    const int cEnd = Cols.end;

    int rc = 0;
    int cc = 0;
    for (int i = rStart; i < rEnd; i = i + rStep) {
        for (int j = cStart; j < cEnd; j = j + cStep) {
            data_[i-1][j-1] = subM[rc][cc];
            cc++;
        }
        rc++;
        cc = 0;
    }
}

// for direct method usage
void mathExtension::Matrix::setSubMatrix\
(\
        const List<int> &Rows, \
        const List<int> &Cols, \
        const mathExtension::Matrix &subM\
)
{
    const int & sizeR = Rows.size();
    const int & sizeC = Cols.size();
    const int & sizeMR = subM.nrow();
    const int & sizeMC = subM.ncol();
    if (sizeR == sizeC && sizeMR == sizeMC && sizeR == sizeMR) {
        for (int i = 0; i < sizeMR; i++) {
            const int & ri = Rows[i];
            for (int j = 0; j < sizeMR; j++) {
                const int & cj = Cols[j];
                data_[ri][cj] = subM[i][j];
            }
        }
    }
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

void mathExtension::Matrix::operator=(const mathExtension::Matrix &m)
{
    if((*this) == m) {
        for (int i = 0; i < nrow(); i++) {
            for (int j = 0; j < ncol(); j++) {
                (*this)[i][j] = m[i][j];
            }
        }
    } else {
        // reset the size of this matrix
        for (int i = 0; i < nrow(); i++) {
            delete [] data_[i];
        }
        delete [] data_;

        const_cast<int&>(nrow_) = m.nrow();
        const_cast<int&>(ncol_) = m.ncol();
        data_ = new double *[nrow()];
        for (int i = 0; i < nrow(); i++) {
            data_[i] = new double[ncol()];
        }

        for (int i = 0; i < nrow(); i++) {
            for (int j = 0; j < ncol(); j++) {
                data_[i][j] = m[i][j];
            }
        }
    }
}

mathExtension::Matrix mathExtension::Matrix::operator+(const mathExtension::Matrix &m) const
{
    Matrix mm(nrow(), ncol());
    if((*this) == m) {

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
    Matrix mm(nrow(), ncol());
    if((*this) == m) {
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
    int nr = nrow();
    int nc = m.ncol();

    Matrix mm(nr, nc);

    if(ncol() == m.nrow()) {
        int ne = ncol();
        for (int i = 0; i < nr; i++) {
            for (int j = 0; j < nc; j++) {
                for (int k = 0; k < ne; k++) {
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

mathExtension::Vector mathExtension::Matrix::operator*(const mathExtension::Vector &v) const
{
    Vector vv(nrow());
    for (int i = 0; i < nrow(); i++) {
        for (int j = 0; j < ncol(); j++) {
            vv[i] = (*this).data()[i][j]*v[j];
        }
    }
    return vv;
}

mathExtension::Matrix mathExtension::Matrix::operator*(const double &s) const
{
    Matrix mm(nrow(), ncol());
    for (int i = 0; i < nrow(); i++) {
        for (int j = 0; j < ncol(); j++) {
            mm[i][j] *= s;
        }
    }
    return mm;
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


mathExtension::Vector::Vector()
    :
      nrow_(1),
      data_(NULL)
{
    data_ = new double[1];
}

mathExtension::Vector::Vector(int nrow)
    :
      nrow_(nrow),
      data_(NULL)
{
    data_ = new double[nrow];
    zeroize();
}

mathExtension::Vector::Vector(int nrow,double *array)
    :
      nrow_(nrow),
      data_(NULL)
{
    data_ = new double[nrow];
    for (int i = 0; i < nrow; i++) {
        data_[i] = array[i];
    }
}

mathExtension::Vector::Vector(const mathExtension::Point &p):
    nrow_(3),
    data_(NULL)
{
    data_ = new double[3];
    data_[0] = p.x();
    data_[1] = p.y();
    data_[2] = p.z();
}

mathExtension::Vector::~Vector()
{
    delete [] data_;
}

void mathExtension::Vector::set(const int i, double val)
{
    data_[i] = val;
}

void mathExtension::Vector::zeroize()
{
    for(int i = 0; i < nrow(); i++) {
        data_[i] = 0.0;
    }
}

//
void mathExtension::Vector::operator=(const mathExtension::Vector &v) const
{
    if(nrow() == v.nrow()) {
        for(int i = 0; i < nrow(); i++) {
            data_[i] = v[i];
        }
    } else {
        std::cout << "Inconsistency of size is not allowed!" << std::endl;
    }
}

mathExtension::Vector mathExtension::Vector::operator+(const mathExtension::Vector &v) const
{
    Vector vv(nrow());
    if(nrow() == v.nrow()) {
        for(int i = 0; i < nrow(); i++) {
            vv[i] = (*this)[i] + v[i];
        }
    } else {
        std::cout << "Inconsistency of size is not allowed!" << std::endl;
    }
    return vv;
}

mathExtension::Vector mathExtension::Vector::operator-(const mathExtension::Vector &v) const
{
    Vector vv(nrow());
    if(nrow() == v.nrow()) {
        for(int i = 0; i < nrow(); i++) {
            vv[i] = (*this)[i] - v[i];
        }
    } else {
        std::cout << "Inconsistency of size is not allowed!" << std::endl;
    }
    return vv;
}

double mathExtension::Vector::operator*(const mathExtension::Vector &v) const
{
    double sum = 0;
    if(nrow() == v.nrow()) {
        for (int i = 0; i < nrow(); i++) {
            sum += (*this)[i]*v[i];
        }
    } else {
        std::cout << "Inconsistency of size is not allowed!" << std::endl;
    }
    return sum;
}

bool mathExtension::Vector::operator==(const mathExtension::Vector &v) const
{
    return ((*this).nrow() == v.nrow());
}

bool mathExtension::Vector::operator!=(const mathExtension::Vector &v) const
{
    return !((*this) == v);
}

double &mathExtension::Vector::operator[](const int i) const
{
    return data_[i];
}

double mathExtension::Vector::cos(const mathExtension::Vector &v)
{
    double n1 = norm(*this);
    double n2 = norm(v);

    double dot = (*this)*v;
    return dot/n1/n2;
}

double mathExtension::Vector::dotProduct(const mathExtension::Vector &v)
{
    return (*this)*v;
}

double mathExtension::Vector::norm(const mathExtension::Vector &v1)
{
    int size = v1.nrow();
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += v1[i]*v1[i];
    }

    return sqrt(sum);
}


mathExtension::Point::Point(double x, double y, double z)
{
    x_ = x;
    y_ = y;
    z_ = z;
}

void mathExtension::Point::operator=(const mathExtension::Vector &v)
{
    if(v.nrow() == 3) {
        x_ = v[0];
        y_ = v[1];
        z_ = v[2];
    }
}
