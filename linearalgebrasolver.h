#ifndef LINEARALGEBRASOLVER_H
#define LINEARALGEBRASOLVER_H

class linearAlgebraSolver
{
public:
    linearAlgebraSolver();
    linearAlgebraSolver(int nl, int nc, double **A, double *b, double *x);
    ~linearAlgebraSolver();
// direct Methods
    void LUSolve();

// indirect Methods

private:
//    double **A_;
//    double *b_;
//    double *x_;



};

#endif // LINEARALGEBRASOLVER_H
