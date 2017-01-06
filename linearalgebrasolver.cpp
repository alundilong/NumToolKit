#include "linearalgebrasolver.h"
#include <QDebug>

linearAlgebraSolver::linearAlgebraSolver()
{
}

linearAlgebraSolver::linearAlgebraSolver(int nl, double **A, double *b, double *x) {

    size_ = nl;
    A_ = A;
    b_ = b;
    x_ = x;

}

linearAlgebraSolver::~linearAlgebraSolver(){

}

void linearAlgebraSolver::LUSolve() {

    int N = size_;

    // LU decomposition
    double **u = new double*[N];
    double **l = new double*[N];
    for (int i = 0; i < N; i++) {
        u[i] = new double [N];
        l[i] = new double [N];
    }

    for (int i = 1; i <= N; i++) {
        u[0][j-1] = A_[0][j-1];
    }

    for (int i = 2; i <= N; i++) {
        l[i-1][0] = A_[i-1][0]/u[i-1][0];
    }

    for (i = 2; i <= N-1; i++) {
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
    for (int i = 2; i <=N; i++) {
        c[i-1] = b_[i-1];
        for(int j = 1; j <= i-1; j++) {
            c[i-1] -= l[i-1][j-1]*c[j-1];
        }
    }

    x_[N-1] = c[N-1]/u[N-1][N-1];
    for (int i = N-1; i <=1; i--) {
        for(int j = i+1; j <= N; j++) {
            x_[i-1] = c[i-1] - u[i-1][j-1]*x_[j-1];
        }
        x_[i-1] /=u[i-1][i-1];
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

//    printA();
//    printb();

    // Back Substitution Algorithm

    x_[N-1] = b_[N-1]/A_[N-1][N-1]; // find the last value

    for (int i = N-1; i >=1; i--) {
        double term = 0;
        for (int j = i + 1; j <= N; j++) {
            term = term + A_[i-1][j-1]*x_[j-1];
        }
        x_[i-1] = (b_[i-1] - term)/A_[i-1][i-1];
        qDebug() << x_[i-1] ;
    }

}

void linearAlgebraSolver::checkSolution()
{
      if (A_ == nullptr) {
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
        qDebug() << "\n";
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
