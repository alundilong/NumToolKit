#include "linearalgebrasolver.h"
#include <iostream>
using namespace std;

linearAlgebraSolver::linearAlgebraSolver()
{
}

linearAlgebraSolver::linearAlgebraSolver(int nl, double **A, double *b, double *x) {

//    A_ = new double*[size_];
//    for (int i = 0; i < size_; i++) {
//        A[i] = new double [size_];
//    }

    size_ = nl;
    A_ = A;
    b_ = b;
    x_ = x;

//    cout << "Constructor of linearAlgebraSolver" << endl;
//    cout << "size of matrix : " << size_ << endl;
//    for (int i = 0; i < size_; i ++) {
//        cout << ".. i .. " << i << endl;
//        for (int j = 0; j < size_; j++) {
//            cout << ".. j .." << j << endl;
//            cout << "ele: " << A_[i][j] << " ";
//        }
//        cout << "\n";
//    }


//    b_ = new double[size_];
//    x_ = new double[size_];
}

linearAlgebraSolver::~linearAlgebraSolver(){
//    for (int i = 0; i < size_; i++) {
//        delete [] A_[i];
//    }
//    delete [] A_;
//    delete [] b_;
//    delete [] x_;
}

void linearAlgebraSolver::LUSolve() {

    int N = size_;

    if (N == 1) {
        x_[0] = b_[0]/A_[0][0];
        return;
    }

    // Gauss Forward Elimination Algorithm
    for (int k = 1; k <= N-1; k++) {

        for (int i = k+1; i <= N; i++) {
            double ratio = A_[i-1][k-1]/A_[k-1][k-1];

            for (int j = k+1; j <= N; j++) {
                A_[i-1][j-1] = A_[i-1][j-1] - ratio * A_[k-1][j-1];
            }

            b_[i-1] = b_[i-1] - ratio*b_[k-1];
            x_[i-1] = b_[i-1];
        }
    }

}
