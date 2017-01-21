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

#include "FeaElementBarThreeD.h"

const int FEAElementBarThreeD::nNode = 8;
const int FEAElementBarThreeD::nDOF = 6;

FEAElementBarThreeD::FEAElementBarThreeD()
{
}

FEAElementBarThreeD::FEAElementBarThreeD \
( \
        const std::string &dimension, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g) \
    :
      FEAElementThreeD(dimension,name,m,g)
{
    log_ = "3D Bar Element is selected \n";
    dim_ = 3;
    nDOFEle_ = 6;
    nNodeEle_ = 8;
    name_ = "Bar3D";
    log_ += QString("%3D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());

    const int N = nNode*nDOF;
//    baseMass_ = new double *[N];
//    baseStiff_ = new double *[N];
//    for (int i = 0; i < N; i++) {
//        baseMass_[i] = new double[N];
//        baseStiff_[i] = new double[N];
//    }

    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

//    const double mass = m.rho()*g.volume();
//    const double *eL = g.e();
//    const double ex = eL[GeometryEle::X];
//    const double coeffMass = mass*ex/6.0;

//    baseMass_[0][0] = 2*coeffMass; baseMass_[0][1] = 1*coeffMass;
//    baseMass_[1][0] = 1*coeffMass; baseMass_[1][1] = 2*coeffMass;

//    const double E = m.E();
//    const double A = g.A();
//    const double coeffStiff = E*A/ex;
//    baseStiff_[0][0] = 1*coeffStiff; baseStiff_[0][1] = -1*coeffStiff;
//    baseStiff_[1][0] = -1*coeffStiff; baseStiff_[1][1] = 1*coeffStiff;

//    // compute moment of inertia
//    const double ey = eL[GeometryEle::Y];
//    const double ez = eL[GeometryEle::Z];

//    I[FEAElementBase::component::XX] = mass*(ey*ey+ez*ez)/12.;
//    I[FEAElementBase::component::YY] = mass*(ex*ex+ez*ez)/12.;
//    I[FEAElementBase::component::ZZ] = mass*(ex*ex+ey*ey)/12.;

    // coordinate system transformation
    // from local to global

    const coordSystem * cs = geometry()->localCoordinateSystem();
    const QVector3D & e0 = cs->e0();
    const QVector3D & e1 = cs->e1();
    const QVector3D & e2 = cs->e2();

    mathExtension::Matrix G(3,3);

    baseStiff_ = G.transpose()*baseStiff_*G;
    baseMass_ = G.transpose()*baseMass_*G;
}

FEAElementBarThreeD::~FEAElementBarThreeD()
{
//    int N = nNode*nDOF;
//    for (int i = 0; i < N; i ++){
//        delete [] baseMass_[i];
//        delete [] baseStiff_[i];
//    }
//    delete [] baseMass_;
//    delete [] baseStiff_;
}

makeElement(ElementName, FEAElementBarThreeD, FEAElementThreeD, BarThreeD)
