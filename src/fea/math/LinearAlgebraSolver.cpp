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
    luBacksbustitude(indx, b());
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

    for(i = 1; i <=n; i++) {
        big = 0.0;
        for (j = 1; j <=n; j++) {
            if ((temp = fabs(A()[i-1][j-1])) > big) big = temp;
        } // end of inner for

        if(big == 0.0) qFatal(" Singular matrix in routine luDecompose");
        vv[i-1] = 1.0/big;
    } // end of outer for

    for(j = 1; j <=n; j++) {
        for (i = 1; i < j; i++) {
            sum = A()[i-1][j-1];
            for (k = 1; k < i; k++) sum -= A()[i-1][k-1]*A()[k-1][j-1];
            A()[i-1][j-1] = sum;
        } // end of inner for

        big = 0.0;
        for (i = j; i <=n; i++) {
            sum = A()[i-1][j-1];
            for (k = 1; k < j; k++)
                sum -= A()[i-1][k-1]*A()[k-1][j-1];
            A()[i-1][j-1] = sum;
            if((dum = vv[i-1]*fabs(sum)) >= big) {
                big = dum;
                imax = i;
            } // end of if
        } // end of outer for
        if(j != imax) {
            for (k=1; k<=n; k++) {
                dum = A()[imax-1][k-1];
                A()[imax-1][k-1] = A()[j-1][k-1];
                A()[j-1][k-1] = dum;
            } // end of for
            *d = -(*d);
            vv[imax-1] = vv[j-1];
        } // end of if

        indx[j-1] = imax;
        if(A()[j-1][j-1] == 0.0) A()[j-1][j-1] = TINY;

        if(j != n) {
            dum = 1.0/A()[j-1][j-1];
            for (i = j+1; i <=n; i++) A()[i-1][j-1] *= dum;
        }
    } // end of outer for

    delete [] vv;
}

void linearAlgebraSolver::luBacksbustitude(const int *indx, Vector &b)
{
    int i, ii = 0, ip, j;
    double sum;
    int n = size();

    for (i = 1; i <=n; i++) {
        ip = indx[i-1];
        sum = b[ip-1];
        b[ip-1] = b[i-1];
        if(ii)
            for (j = ii; j <= i-1; j++) sum -= A()[i-1][j-1]*b[j-1];
        else if (sum) ii = i;
        b[i-1] = sum;
    }
    for (i = n; i>=1; i--) {
        sum = b[i-1];
        for (j = i+1; j <= n; j++) sum -= A()[i-1][j-1]*b[j-1];
        b[i-1] = sum/(A()[i-1][i-1]);
    }
    x() = b;
}

void linearAlgebraSolver::LUSolve_GSL()
{
    double *a_data = new double[size()*size()];
    double *b_data = new double[size()];
    int N = size();
    int c = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a_data[c] = A()[i][j];
            c++;
        }
    }
    for (int i = 0; i < N; i++) {
        b_data[i] = b()[i];
    }

    gsl_matrix_view m
      = gsl_matrix_view_array (a_data, N, N);
    gsl_vector_view b
      = gsl_vector_view_array (b_data, N);
    gsl_vector *xval = gsl_vector_alloc (N);
    int s;
    gsl_permutation * p = gsl_permutation_alloc (N);
    gsl_linalg_LU_decomp (&m.matrix, p, &s);
    gsl_linalg_LU_solve (&m.matrix, p, &b.vector, xval);
    for (int i = 0; i < N; i++) {
        x()[i] = gsl_vector_get(xval, i);
    }

    gsl_permutation_free (p);
    gsl_vector_free (xval);

    delete[] a_data;
    delete[] b_data;
}

void linearAlgebraSolver::GaussElimination()
{
    // row pivoting
    int   i,j,k,m,rowx;
    double xfac,temp,temp1,amax;

    int n = size();
    rowx = 0;
    for (k=1; k<=n-1; ++k) {
        amax = (double) fabs(A()[k-1][k-1]) ;
        m = k;
        for (i=k+1; i<=n; i++){
            xfac = (double) fabs(A()[i-1][k-1]);
            if(xfac > amax) {amax = xfac; m=i;}
        }
        if(m != k) {
            rowx = rowx+1;
            temp1 = b()[k];
            b()[k-1]  = b()[m-1];
            b()[m-1]  = temp1;
            for(j=k; j<=n; j++) {
                temp = A()[k-1][j-1];
                A()[k-1][j-1] = A()[m-1][j-1];
                A()[m-1][j-1] = temp;
            }
        }
        for (i=k+1; i<=n; ++i) {
            xfac = A()[i-1][k-1]/A()[k-1][k-1];

            for (j=k+1; j<=n; ++j) {
                A()[i-1][j-1] = A()[i-1][j-1]-xfac*A()[k-1][j-1];
            }
            b()[i-1] = b()[i-1]-xfac*b()[k-1];
        }
    }

    for (j=1; j<=n; ++j) {
        k=n-j+1;
        x()[k-1] = b()[k-1];
        for(i=k+1; i<=n; ++i) {
            x()[k-1] = x()[k-1]-A()[k-1][i-1]*x()[i-1];
        }
        x()[k-1] = x()[k-1]/A()[k-1][k-1];
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
    const int & N = size();
    // initial guess
    for (int i = 1; i <= N; i++) {
        x_[i-1] = 0;
    }

    int nIter = 0;
    double maxRes = 1e6;

//    while ((nIter < nIterMax()) && (maxRes > tolerance())) {
//        nIter++;
//        double maxResPrevElement = 0.0;
//        for (int i = 1; i <= N; i++) {
//            double tmp = b()[i-1];
//            for (int j = 1; j <= N; j++) {
//                if (i == j) continue;
//                if(j <= i-1) {
//                    tmp -= A()[i-1][j-1]*x()[j-1]; // using current value
//                } else if (j >= (i+1)) {
//                    tmp -= A()[i-1][j-1]*x()[j-1]; // using previous value
//                }
//            }
//            double xPrev = x()[i-1];
//            x()[i-1] = tmp/A()[i-1][i-1];
//            double xNew = x()[i-1];
//            maxRes = fmax(fabs((xNew-xPrev)/xNew)*100, maxResPrevElement);
//            maxResPrevElement = maxRes;
//        }
//    }

    mathExtension::Vector y(N);
    double error = 1e6;
    while(nIter<nIterMax() && error > tolerance()) {
        for (int i = 0; i < N; i++)
        {
            y[i] = (b()[i] / A()[i][i]);
            for (int j = 0; j < N; j++)
            {
                if (j == i)
                    continue;
                y[i] = y[i] - ((A()[i][j] / A()[i][i]) * x()[j]);
                x()[i] = y[i];
            }
//            std::cout << "x" << i+1 << " = " << x()[i] << " " << y[i] << std::endl;
        }
        nIter++;
    }
    x() = y;
}

double linearAlgebraSolver::determinant(const Matrix &A)
{
    // very expensive
    // good for n < 10
    double d = 0.0;
    int c, subi, i, j, subj;
    int n = A.nrow();
    mathExtension::Matrix subA(n-1,n-1);

    if (n == 2)
    {
        return( (A[0][0] * A[1][1]) - (A[1][0] * A[0][1]));
    }
    else
    {
        for(c = 0; c < n; c++)
        {
            subi = 0;
            for(i = 1; i < n; i++)
            {
                subj = 0;
                for(j = 0; j < n; j++)
                {
                    if (j == c)
                    {
                        continue;
                    }
                    subA[subi][subj] = A[i][j];
                    subj++;
                }
                subi++;
            }
            double coeff = 1.0;
            if(c%2!=0) coeff = -1.0;
            d = d + (coeff * A[0][c] * determinant(subA));
        }
    }
    return d;
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
