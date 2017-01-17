#include "feaelementlinearcubicalelement.h"


const int FEAElementLinearCubicalElement::nNode = 8;
const int FEAElementLinearCubicalElement::nDOF = 3;

FEAElementLinearCubicalElement::FEAElementLinearCubicalElement()
{
}

FEAElementLinearCubicalElement::FEAElementLinearCubicalElement\
(\
        const std::string &dimension, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g
)
    : FEAElementThreeD(dimension, name, m, g)
{
    log_ = "3D LinearCubicalElement is selected \n";
    dim_ = 3;
    nDOFEle_ = 3;
    nNodeEle_ = 8;
    name_ = "LinearCubicalElement3D";
    log_ += QString("%3D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());

    const int N = nNode*nDOF;
    baseMass_ = new double *[N];
    baseStiff_ = new double *[N];
    for (int i = 0; i < N; i++) {
        baseMass_[i] = new double[N];
        baseStiff_[i] = new double[N];
    }
}

FEAElementLinearCubicalElement::~FEAElementLinearCubicalElement()
{
    int N = nNodeEle_;
    for (int i = 0; i < N; i ++){
        delete [] baseMass_[i];
        delete [] baseStiff_[i];
    }
    delete [] baseMass_;
    delete [] baseStiff_;
}

makeElement(ElementName, \
            FEAElementLinearCubicalElement, \
            FEAElementThreeD, \
            LinearCubicalElementBarThreeD)
