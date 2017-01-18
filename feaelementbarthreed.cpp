#include "feaelementbarthreed.h"

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
    baseMass_ = new double *[N];
    baseStiff_ = new double *[N];
    for (int i = 0; i < N; i++) {
        baseMass_[i] = new double[N];
        baseStiff_[i] = new double[N];
    }

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
}

FEAElementBarThreeD::~FEAElementBarThreeD()
{
    int N = nNode*nDOF;
    for (int i = 0; i < N; i ++){
        delete [] baseMass_[i];
        delete [] baseStiff_[i];
    }
    delete [] baseMass_;
    delete [] baseStiff_;
}

makeElement(ElementName, FEAElementBarThreeD, FEAElementThreeD, BarThreeD)
