#ifndef MATHEXTENSION_H
#define MATHEXTENSION_H

#include <iostream>

namespace mathExtension {

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

#include "mathExtensionTemplate.h"

// Class Matrix defined to handle matrix operation

class Matrix {

public:

    Matrix();
    Matrix(int nrow, int ncol);
    // construct from
    Matrix(int nr, int nc, double **array);
//    Matrix(int nr, int nc, double array[][nc]);
    ~Matrix();

private:

    const int nrow_, ncol_;
    double **data_;
    void zeroize();
    inline const int nrow() const { return nrow_;}
    inline const int ncol() const { return ncol_;}
    inline double **data() const { return data_;}

public:

    // handling small matrix
    // therefore return a copy only
    // transpose (no change to the original)
    Matrix transpose();

    // operator overload
    // copy
    void operator=(const Matrix &m) const;
    // addition
    Matrix operator+(const Matrix &m) const;
    // subtraction
    Matrix operator-(const Matrix &m) const;
    // multiplication
    Matrix operator*(const Matrix &m) const;

    // equivalence (check size only)
    bool operator==(const Matrix &m) const;
    // equivalence (check size only)
    bool operator!=(const Matrix &m) const;

    // get value
    double *operator[](const int i) const;
};


// gauss point and weight look-up table

long double gaussP(int nSize, int p);
long double gaussW(int nSize, int p);

}


#endif // MATHEXTENSION_H
