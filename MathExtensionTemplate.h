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

#ifndef MATHEXTENSIONTEMPLATE_H
#define MATHEXTENSIONTEMPLATE_H

// zeroize vector
template <typename T , size_t r1>
void vectorZero(T (&array1)[r1]){
    for (int i = 0; i < r1; i++) {
        array1[i] = 0.0;
    }
}

// Matrix zeroize
template <typename T , size_t r1, size_t c1>
void matrixZero(T (&array1)[r1][c1]){

    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c1; j++) {
            array1[i][j] = 0.0;
        }
    }
}

// Matrix transpose (no change to original matrix)
template <typename T , size_t r1, size_t c1>
void matrixTranspose(T (&array1)[r1][c1], T (&array2)[c1][r1]){

    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c1; j++) {
            array2[i][j] = array1[j][i];
        }
    }
}

// Matrix X Matrix (no changes to original matrix)
template <typename T , size_t r1, size_t c1, size_t r2, size_t c2>
void matrixMultiplyMatrix(T (&array1)[r1][c1], T(&array2)[r2][c2], T(&array)[r1][c2])
{
    if(c1 != r2) {
        std::cout << "Error: Not Allowed to perform matrix-matrix multiplication!" << std::endl;
        std::cout << c1 << " != " << r2 << std::endl;
    }

    for (size_t i = 0; i < r1; ++i)
    {
        for (size_t j = 0; j < c2; ++j) {
            for (size_t k = 0; k < c1; k++ )
                array[i][j] += array1[i][k]*array2[k][j];
        }
    }
}

// Matrix x vector (no changes to the orginal matrix)
template <typename T , size_t r1, size_t c1, size_t r2>
void matrixMultiplyVector(T (&array1)[r1][c1], T(&array2)[r2], T(&array)[r1])
{
    if(c1 != r2) {
        std::cout << "Error: Not Allowed to perform matrix-vector multiplication!" << std::endl;
        std::cout << c1 << " != " << r2 << std::endl;
    }

    for (size_t i = 0; i < r1; ++i)
    {
        for (size_t j = 0; j < c1; ++j) {
            array[i] += array1[i][j]*array2[j];
        }
    }
}

#endif // MATHEXTENSIONTEMPLATE_H
