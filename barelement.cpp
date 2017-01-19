#include "barelement.h"

//const std::string BarElement::typeName = "Bar";
const int BarElement::nNode = 2;
const int BarElement::nDOF = 1;

BarElement::BarElement() : FEAElementOneD()
{
    log_ = "Bar Element is selected \n";
    dim_ = 1;
    nDOFEle_ = 1;
    nNodeEle_ = 2;
    name_ = "Bar";
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

    baseMass_[0][0] = 2; baseMass_[0][1] = 1;
    baseMass_[1][0] = 1; baseMass_[1][1] = 2;

    baseStiff_[0][0] = 1; baseStiff_[0][1] = -1;
    baseStiff_[1][0] = -1; baseStiff_[1][1] = 1;

    // The material and geoinfo are null
    if (material() == nullptr) {
        qDebug() << "Material for each Element are unknown!";
    }

    if (geometry() == nullptr) {
        qDebug() << "GeoInfo for each Element are unknown!";
    }
}

BarElement::BarElement
(
        const std::string & dimension,
        const std::string & name,
        const MaterialEle &m,
        const GeometryEle &g
) :
    FEAElementOneD(dimension, name, m, g)
{
    log_ = "OnD-Bar Element is selected \n";
    dim_ = 1;
    nDOFEle_ = 1;
    nNodeEle_ = 2;
    name_ = "Bar";
    log_ += QString("%1D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());

    const int N = nNode*nDOF;

    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

    const double mass = m.rho()*g.volume();
    const double *eL = g.e();
    const double ex = eL[GeometryEle::X];
    const double coeffMass = mass*ex/6.0;

    baseMass_[0][0] = 2*coeffMass; baseMass_[0][1] = 1*coeffMass;
    baseMass_[1][0] = 1*coeffMass; baseMass_[1][1] = 2*coeffMass;

    const double E = m.E();
    const double A = g.A();
    const double coeffStiff = E*A/ex;

    baseStiff_[0][0] = 1*coeffStiff; baseStiff_[0][1] = -1*coeffStiff;
    baseStiff_[1][0] = -1*coeffStiff; baseStiff_[1][1] = 1*coeffStiff;

    // compute moment of inertia
    const double ey = eL[GeometryEle::Y];
    const double ez = eL[GeometryEle::Z];

    I[FEAElementBase::component::XX] = mass*(ey*ey+ez*ez)/12.;
    I[FEAElementBase::component::YY] = mass*(ex*ex+ez*ez)/12.;
    I[FEAElementBase::component::ZZ] = mass*(ex*ex+ey*ey)/12.;

}

BarElement::~BarElement()
{
//    int N = nNode*nDOF;
//    for (int i = 0; i < N; i ++){
//        delete [] baseMass_[i];
//        delete [] baseStiff_[i];
//    }
//    delete [] baseMass_;
//    delete [] baseStiff_;
}

makeElement(ElementName, BarElement, FEAElementOneD, Bar)

//FEAElementOneD::addElementNameConstructorToTable<BarElement>
//addBarElementElementNameConstructorToFEAElementOneDTable_;

