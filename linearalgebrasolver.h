#ifndef LINEARALGEBRASOLVER_H
#define LINEARALGEBRASOLVER_H

#include <QString>

class linearAlgebraSolver
{
public:
    linearAlgebraSolver();
    linearAlgebraSolver(int size, double **A, double *b, double *x);
    ~linearAlgebraSolver();
// direct Methods
    void LUSolve();
    void GaussElimination();

// indirect Methods
    void JacobiMethod();
    void GaussSeidelMethod();

// check results
    void checkSolution();
    void printA();
    void printb();
    inline QString mylog() {return log_;}

private:
    int size_;
    double **A_;
    double *b_;
    double *x_;
    int nIterMax_ = 1000;
    double tolerance_ = 1e-3;
    QString log_ = "";

    inline int nIterMax() {return nIterMax_;}
    inline double tolerance() {return tolerance_;}


};

#endif // LINEARALGEBRASOLVER_H
