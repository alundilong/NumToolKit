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

#ifndef MATHEXTENSION_H
#define MATHEXTENSION_H

#include <iostream>
#include "math.h"
#include "../../container/List.h"
#include <QList>

using namespace NumToolKit;

namespace mathExtension {

struct pos {
    int start;
    int step;
    int end;
};

// Matrix changed, useful in forming base Matrix for each element
void matrixMultiplyScalar(double **m, int nrow, int ncol, double s);// m will be modified

//void matrixMultiplyMatrix(double **m, int nrow_m, int ncol_m, double **n, int nrow_n, int ncol_n);
//void matrixMultiplyVector(double **m, int nrow_m, int ncol_n, double *n, int nrow_n);

// zeroize vector
template <typename T , size_t r1>
void vectorZero(T (&array1)[r1]);

// zeroize matrix
template <typename T , size_t r1, size_t c1>
void matrixZero(T (&array1)[r1][c1]);

// transpose matrix return void
template <typename T , size_t r1, size_t c1>
void matrixTranspose(T (&array1)[r1][c1], T (&array2)[c1][r1]);

// transpose matrix return a matrix
// it would be good to define this
// to shorten code in base matrix creation
// template <typename T , size_t r1, size_t c1>
// T** matrixTranspose(T (&array1)[r1][c1]);

// M x M
template <typename T , size_t r1, size_t c1, size_t r2, size_t c2>
void matrixMultiplyMatrix(T (&array1)[r1][c1], T(&array2)[r2][c2], T(&array)[r1][c2]);

// M x V
template <typename T , size_t r1, size_t c1, size_t r2>
void matrixMultiplyVector(T (&array1)[r1][c1], T(&array2)[r2], T(&array)[r1]);

// definition of all template functions

#include "MathExtensionTemplate.h"

int imax(const int &a, const int &b);
int imin(const int &a, const int &b);

// Class Vector defined to handle vector operation

class Point;
class Vector {

public:

    Vector();
    Vector(int nrow);
    // construct from
    Vector(int nr, double *array);
    // construct form Point
    Vector(const Point &p);
    ~Vector();
    void set(const int i, double val);
    inline const int nrow() const { return nrow_;}


private:

    const int nrow_;
    double *data_;
    void zeroize();

    inline double *data() const { return data_;}

public:

    // operator overload
    // copy
    void operator=(const Vector &v) const;
    // addition
    Vector operator+(const Vector &v) const;
    // subtraction
    Vector operator-(const Vector &v) const;
    // dot product
    double operator*(const Vector &v) const;

    // equivalence (check size only)
    bool operator==(const Vector &v) const;
    // equivalence (check size only)
    bool operator!=(const Vector &v) const;

    // get value
    double &operator[](const int i) const;

    friend std::ostream &operator<<(std::ostream& os, const Vector&v){
        int nr = v.nrow();
        for(int i = 0; i < nr; i++ ) {
            os << v[i] << ' ';
        }
        return os;
    }

    // find cos of two vector
    double cos(const Vector &v);
    double dotProduct(const Vector &v);
    double norm(const Vector &v1);
};


// Class Matrix defined to handle matrix operation

class Matrix {

public:

    Matrix();
    Matrix(int nrow, int ncol);
    // construct from
    Matrix(int nr, int nc, double **array);
//    Matrix(int nr, int nc, double array[][nc]);
    ~Matrix();

    void set(const int i, const Vector &v);
    void setColValues(const int iCol, const pos & Cols, const Vector & v, const bool & shift);
    void setSubMatrix(const pos & Rows, const pos & Cols, const Matrix & subM, const bool&shift);
    void assemblyMatrix(const QList<int> & Rows, const QList<int> & Cols, const Matrix & subM, const bool & shift, const int &);
    void setZeroExceptRowCol(const int & Row, const int &Col);
    inline const int nrow() const { return nrow_;}
    inline const int ncol() const { return ncol_;}

private:

// disallow copy constructor
//    Matrix(const Matrix& m) = delete;
    const int nrow_, ncol_;
    double **data_;
    void zeroize();

    inline double **data() const { return data_;}

public:

    // handling small matrix
    // therefore return a copy only
    // transpose (no change to the original)
    Matrix transpose();
    // make change to the original matrix
    void symmetry();

    // operator overload
    // copy
    void operator=(const Matrix &m);
    // addition
    Matrix operator+(const Matrix &m) const;
    // subtraction
    Matrix operator-(const Matrix &m) const;
    // multiplication x matrix
    Matrix operator*(const Matrix &m) const;
    // multiplication x vector
    Vector operator*(const Vector &v) const;
    // multiplication x scalar
    Matrix operator*(const double &s) const;

    // equivalence (check size only)
    bool operator==(const Matrix &m) const;
    // equivalence (check size only)
    bool operator!=(const Matrix &m) const;

    // get value
    const double *operator[](const int i) const;
    double *operator[](const int i);

    friend std::ostream &operator<<(std::ostream& os, const Matrix&m){
        int nr = m.nrow();
        int nc = m.ncol();
        for(int i = 0; i < nr; i++ ) {
            for(int j = 0; j < nc; j++) {
                os << m[i][j] << ' ';
            }
            os << '\n';
        }
        return os;
    }
};

class sparseMatrix {

public:
    sparseMatrix();
    ~sparseMatrix();

private:
    int *lowPtr_,*diaPtr_, *upperPtr_;

};


class Point {
public:
    Point();
    Point(double x, double y, double z);
    ~Point();

    const double x() const { return x_;}
    const double y() const { return y_;}
    const double z() const { return z_;}

    void operator=(const Vector &v);
    Point operator-(const Point &p) const;
    double operator[](const int &index) const;

    friend std::ostream &operator<<(std::ostream& os, const Point&p){
        os << p.x() << ' ' << p.y() << ' ' << p.z() << '\n';
        return os;
    }

private:
    double x_;
    double y_;
    double z_;
};

//#include "mathExtensionIO.h"
// gauss point and weight look-up table

long double gaussP(int nSize, int p);
long double gaussW(int nSize, int p);


}


#endif // MATHEXTENSION_H
