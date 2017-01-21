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

#ifndef LINEARALGEBRASOLVER_H
#define LINEARALGEBRASOLVER_H

#include <QString>


namespace NumToolKit {

namespace Fea {


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
    void printx();
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

}

}

#endif // LINEARALGEBRASOLVER_H
