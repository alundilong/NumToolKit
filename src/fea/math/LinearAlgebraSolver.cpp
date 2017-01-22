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

    int N = size_;
    if (N == 1) {
        x_[0] = b_[0]/A_[0][0];
        return;
    }

    // LU decomposition
    double **u = new double*[N];
    double **l = new double*[N];
    for (int i = 0; i < N; i++) {
        u[i] = new double [N];
        l[i] = new double [N];
    }

    for (int j = 1; j <= N; j++) {
        u[0][j-1] = A_[0][j-1];
    }

    for (int i = 2; i <= N; i++) {
        l[i-1][0] = A_[i-1][0]/u[0][0];
    }

    for (int i = 2; i <= N-1; i++) {
        for (int j = i; j <= N; j++) {
            u[i-1][j-1] = A_[i-1][j-1];
            for (int k = i+1; k <= N; k++) {
                u[i-1][j-1] -= l[i-1][k-1]*u[k-1][j-1];
                l[k-1][i-1] = (A_[k-1][i-1] - l[k-1][j-1]*u[j-1][i-1])/u[i-1][i-1];
            }
        }
    }

    u[N-1][N-1] = A_[N-1][N-1];
    for (int i = 1; i <= N-1; i++) {
        u[N-1][N-1] -= l[N-1][i-1]*u[i-1][N-1];
    }

    // the substitution step
    double *c = new double[N];
    c[0] = b_[0];
    for (int i = 2; i <= N; i++) {
        c[i-1] = b_[i-1];
        for(int j = 1; j <= i-1; j++) {
            c[i-1] -= l[i-1][j-1]*c[j-1];
        }
    }


    x_[N-1] = c[N-1]/u[N-1][N-1];

    for (int i = N-1; i >= 1; i--) {
        double term = c[i-1];
        for(int j = i+1; j <= N; j++) {
             term -= u[i-1][j-1]*x_[j-1];
        }
        x_[i-1] = term/u[i-1][i-1];
    }

    for (int i = 0; i < N; i++) {
        delete [] u[i];
        delete [] l[i];
    }
    delete [] c;
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
