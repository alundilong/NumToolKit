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

#include "LinearAlgebraSolver.h"
#include <QDebug>
#include "math.h"

#define TINY 1.0e-20;

namespace NumToolKit {

namespace Fea {

//linearAlgebraSolver::linearAlgebraSolver()
//{
//}

linearAlgebraSolver::linearAlgebraSolver(Matrix &A, Vector &b, Vector &x)
    :
      A_(A),
      b_(b),
      x_(x)
{
    size_ = A.nrow();
}

linearAlgebraSolver::~linearAlgebraSolver(){

}

void linearAlgebraSolver::LUSolve() {

    int *indx;
    double d;
    indx = new int[size()];
    luDecompose(indx,&d);
    luBacksbustitude(indx, x());
    delete [] indx;
}

void linearAlgebraSolver::luDecompose(int *indx, double *d)
{
    int i, imax, j, k;
    double big, dum, sum, temp;
    double *vv;
    int n = size();
    vv = new double[n];
    *d = 1.0;
    for(i = 0; i <=n; i++) {
        big = 0.0;
        for (j = 0; j <=n; j++) {
            if ((temp = fabs(A()[i][j])) > big) big = temp;
        } // end of inner for

        if(big == 0.0) qFatal(" Singular matrix in routine luDecompose");
        vv[i] = 1.0/big;
    } // end of outer for

    for(j = 0; j <=n; j++) {
        for (i = 0; i < j; i++) {
            sum = A()[i][j];
            for (k = 1; k < i; k++) sum -= A()[i][k]*A()[k][j];
        } // end of inner for
        A()[i][j] = sum;

        big = 0.0;
        for (i = j; i <=n; i++) {
            sum = A()[i][j];
            for (k = 1; k < j; k++) {
                sum -= A()[i][k]*A()[k][j];
            } // end of inner for
            A()[i][j] = sum;
            if((dum = vv[i]*fabs(sum)) >= big) {
                big = dum;
                imax = i;
            } // end of if
        } // end of outer for
        if(j != imax) {
            for (k=1; k<=n; k++) {
                dum = A()[imax][k];
                A()[imax][k] = A()[j][k];
                A()[j][k] = dum;
            } // end of for
            *d = -(*d);
            vv[imax] = vv[j];
        } // end of if

        indx[j] = imax;
        if(A()[j][j] == 0.0) A()[j][j] = TINY;

        if(j != n) {
            dum = 1.0/A()[j][j];
            for (i = j+1; i <=n; i++) A()[i][j] *= dum;
        }
    } // end of outer for

    delete [] vv;
}

void linearAlgebraSolver::luBacksbustitude(const int *indx, Vector &x)
{
    int i, ii = 0, ip, j;
    double sum;
    int n = size();

    for (i = 1; i <=n; i++) {
        ip = indx[i];
        sum = x[ip];
        x[ip] = x[i];
        if(ii)
            for (j = ii; j <= i-1; j++) sum -= A()[i][j]*x[j];
        else if (sum) ii = i;
        x[i] = sum;
    }
    for (i = n; i>=1; i--) {
        sum = x[i];
        for (j = i+1; j <= n; j++) sum -= A()[i][j]*x[j];
        x[i] = sum/A()[i][i];
    }
}

void linearAlgebraSolver::GaussElimination()
{

    int N = size_;

    // if 1x1 matrix
    if (N == 1) {
        x_[0] = b_[0]/A_[0][0];
        return;
    }

    // Gauss Forward Elimination Algorithm
    for (int k = 1; k <= N-1; k++) {

        for (int i = k + 1; i <= N; i++) {
            double ratio = A_[i-1][k-1]/A_[k-1][k-1];

            for (int j = k + 1; j <= N; j++) {
                A_[i-1][j-1] = A_[i-1][j-1] - ratio * A_[k-1][j-1];
            }

            b_[i-1] = b_[i-1] - ratio*b_[k-1];
        }
    }

    // Back Substitution Algorithm

    x_[N-1] = b_[N-1]/A_[N-1][N-1]; // find the last value

    for (int i = N-1; i >=1; i--) {
        double term = 0;
        for (int j = i + 1; j <= N; j++) {
            term = term + A_[i-1][j-1]*x_[j-1];
        }
        x_[i-1] = (b_[i-1] - term)/A_[i-1][i-1];
    }

}

void linearAlgebraSolver::JacobiMethod()
{
    int N = size_;
    // initial guess
    for (int i = 1; i <= N; i++) {
        x_[i-1] = 0;
    }

    int nIter = 0;
    double maxRes = 1e6;

    while ((nIter < nIterMax()) && (maxRes > tolerance())) {
        nIter++;
        double maxResPrevElement = 0;
        for (int i = 1; i <= N; i++) {
            double tmp = b_[i-1];
            for (int j = 1; j <= N; j++) {
                if (i == j) continue;
                tmp -= A_[i-1][j-1]*x_[j-1];
            }

            double xPrev = x_[i-1];
            x_[i-1] = tmp/A_[i-1][i-1];
            double xNew = x_[i-1];
            maxRes = fmax(fabs((xNew-xPrev)/xNew)*100, maxResPrevElement);
            maxResPrevElement = maxRes;
        }
    }

    if (maxRes > tolerance())
        log_ += QString("Warning : After %1 iterations (maxIteration), the solution is NOT found! \n").arg(nIterMax());
    else
        log_ += QString("After %1 iterations, the solution is found! \n").arg(nIter);

}

void linearAlgebraSolver::GaussSeidelMethod()
{
    int N = size_;
    // initial guess
    for (int i = 1; i <= N; i++) {
        x_[i-1] = 0;
    }

    int nIter = 0;
    double maxRes = 1e6;

    while ((nIter < nIterMax()) && (maxRes > tolerance())) {
        nIter++;
        double maxResPrevElement = 0;
        for (int i = 1; i <= N; i++) {
            double tmp = b_[i-1];
            for (int j = 1; j <= N; j++) {
                if (i == j) continue;
                if(j <= i-1) {
                    tmp -= A_[i-1][j-1]*x_[j-1]; // using current value
                } else if (j >= (i+1)) {
                    tmp -= A_[i-1][j-1]*x_[j-1]; // using previous value
                }
            }

            double xPrev = x_[i-1];
            x_[i-1] = tmp/A_[i-1][i-1];
            double xNew = x_[i-1];
            maxRes = fmax(fabs((xNew-xPrev)/xNew)*100, maxResPrevElement);
            maxResPrevElement = maxRes;
        }
    }
}

double linearAlgebraSolver::determinant(const Matrix &A)
{

}

void linearAlgebraSolver::checkSolution()
{
      if ( A_.nrow() == 0 || A_.ncol() == 0) {
          qDebug() << "A is null!!!";
      }
//    int N = size_;
//    double res[N];
//    for (int i = 0; i < N; i++) {
//        res[i] = 0;
//        for (int j = 0; j < N; j++) {
//            res[i] += A_[i][j]*b_[j];
//        }
//    }
}

void linearAlgebraSolver::printA()
{
    qDebug() << "Print Coefficient Matrix A";

    int N = size_;

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
          qDebug() << A_[i][j];
     }
    }
}

void linearAlgebraSolver::printb()
{
    qDebug() << "Print vector b";
    int N = size_;
    for (int i = 0; i < N; i++) {
      qDebug() << b_[i];
    }
}

void linearAlgebraSolver::printx()
{
    qDebug() << "Print vector x";
    int N = size_;
    for (int i = 0; i < N; i++) {
      qDebug() << x_[i];
    }
}

}

}
