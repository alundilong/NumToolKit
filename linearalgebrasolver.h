#ifndef LINEARALGEBRASOLVER_H
#define LINEARALGEBRASOLVER_H

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

// check results
    void checkSolution();
    void printA();
    void printb();

private:
    int size_;
    double **A_;
    double *b_;
    double *x_;

};

#endif // LINEARALGEBRASOLVER_H
