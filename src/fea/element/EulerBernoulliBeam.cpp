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

#include "EulerBernoulliBeam.h"

namespace NumToolKit {

namespace Fea {

//const std::string EulerBernoulliBeam::typeName = "EulerBernoulliBeam";
const int EulerBernoulliBeam::nNode = 2;
const int EulerBernoulliBeam::nDOF = 2;

EulerBernoulliBeam::EulerBernoulliBeam()
{
    log_ = "Euler-Bernoulli Beam Element is selected \n";
    dim_ = 1;
    nDOFEle_ = 2;
    nNodeEle_ = 2;
    name_ = "EulerBernoulliBeam";
    log_ += QString("%1D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());

    int N = nNodeEle()*nNodeEle();
//    baseMass_ = new double *[N];
//    baseStiff_ = new double *[N];
//    for (int i = 0; i < N; i++) {
//        baseMass_[i] = new double[N];
//        baseStiff_[i] = new double[N];
//    }
    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

//    baseMass_[0][0] = 2; baseMass_[0][1] = 1;
//    baseMass_[1][0] = 1; baseMass_[1][1] = 2;

//    baseStiff_[0][0] = 1; baseStiff_[0][1] = -1;
//    baseStiff_[1][0] = -1; baseStiff_[1][1] = 1;

    // The material and geoinfo are null
    if (material() == nullptr) {
        qDebug() << "Material for each Element are unknown!";
    }

    if (geometry() == nullptr) {
        qDebug() << "GeoInfo for each Element are unknown!";
    }

}

EulerBernoulliBeam::EulerBernoulliBeam
(
        const std::string & dimension,
        const std::string & name,
        const MaterialEle &m,
        const GeometryEle &g
):
    FEAElementOneD(dimension, name, m, g)
{
    log_ = "Euler-Bernoulli Beam Element is selected \n";
    dim_ = 1;
    nDOFEle_ = 2;
    nNodeEle_ = 2;
    name_ = "EulerBernoulliBeam";
    log_ += QString("%1D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());

    int N = nNodeEle()*nNodeEle();
//    baseMass_ = new double *[N];
//    baseStiff_ = new double *[N];
//    for (int i = 0; i < N; i++) {
//        baseMass_[i] = new double[N];
//        baseStiff_[i] = new double[N];
//    }
    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

    const double mass = m.rho()*g.volume();
    const double *eL = g.e();
    const double ex = eL[GeometryEle::X];
    const double coeffMass = mass*ex/420.0;

    baseMass_[0][0] = 156;      baseMass_[0][1] = 22*ex;      baseMass_[0][2] = 54;       baseMass_[0][3] = -13.0*ex;
    baseMass_[1][0] = 22*ex;    baseMass_[1][1] = 4*ex*ex;    baseMass_[1][2] = 13*ex;    baseMass_[1][3] = -3.0*ex*ex;
    baseMass_[2][0] = 54;       baseMass_[2][1] = 13*ex;      baseMass_[2][2] = 156;      baseMass_[2][3] = -22.0*ex;
    baseMass_[3][0] = -13.0*ex; baseMass_[3][1] = -3.0*ex*ex; baseMass_[3][2] = -22.0*ex; baseMass_[3][3] = 4*ex*ex;

//    mathExtension::matrixMultiplyScalar(baseMass_, 4, 4, coeffMass);
    baseMass_ = baseMass_*coeffMass;

    // compute moment of inertia
    const double ey = eL[GeometryEle::Y];
    const double ez = eL[GeometryEle::Z];

    I[FEAElementBase::component::XX] = mass*(ey*ey+ez*ez)/12.;
    I[FEAElementBase::component::YY] = mass*(ex*ex+ez*ez)/12.;
    I[FEAElementBase::component::ZZ] = mass*(ex*ex+ey*ey)/12.;
    const double E = m.E();
    const double Ixx = I[FEAElementBase::component::XX];
    const double coeffStiff = E*Ixx/ex/ex/ex;

    baseStiff_[0][0] = 12;   baseStiff_[0][1] = 6*ex;      baseStiff_[0][2] = -12;     baseStiff_[0][3] = 6.0*ex;
    baseStiff_[1][0] = 6*ex; baseStiff_[1][1] = 4*ex*ex;   baseStiff_[1][2] = -6*ex;   baseStiff_[1][3] = 2.0*ex*ex;
    baseStiff_[2][0] = -12;  baseStiff_[2][1] = -6*ex;     baseStiff_[2][2] = 12;      baseStiff_[2][3] = -6.0*ex;
    baseStiff_[3][0] = 6*ex; baseStiff_[3][1] = 2.0*ex*ex; baseStiff_[3][2] = -6.0*ex; baseStiff_[3][3] = 4*ex*ex;

//    mathExtension::matrixMultiplyScalar(baseStiff_, 4, 4, coeffStiff);
    baseStiff_ = baseMass_*coeffStiff;

}

EulerBernoulliBeam::~EulerBernoulliBeam()
{
//    int N = nNode*nDOF;
//    for (int i = 0; i < N; i ++){
//        delete [] baseMass_[i];
//        delete [] baseStiff_[i];
//    }
//    delete [] baseMass_;
    //    delete [] baseStiff_;
}

void EulerBernoulliBeam::infoAboutThisElement()
{

}

void EulerBernoulliBeam::constructGeometry()
{

}

void EulerBernoulliBeam::constructBaseMatrix()
{

}

void EulerBernoulliBeam::transformToGlobal()
{

}

void EulerBernoulliBeam::numberSequence()
{

}

makeElement(ElementName, EulerBernoulliBeam, FEAElementOneD, EulerBernoulliBeam)
makeElementNode(ElementName, EulerBernoulliBeam, FEAElementBase)
makeElementNodeDOF(ElementName, EulerBernoulliBeam, FEAElementBase)
//FEAElementOneD::addElementNameConstructorToTable<EulerBernoulliBeam>
//addEulerBernoulliBeamElementNameConstructorToFEAElementOneDTable_;

}

}
