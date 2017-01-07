#include "barelement.h"

BarElement::BarElement()
{
    log_ = "Bar Element is selected \n";
    log_ += "Dimension: 1D \n DOF : 1 \n";
    dim_ = 1;
    nDOF_ = 1;
    nSize_ = 2;
    name_ = "Bar";

    int N = nSize_;
    baseMass_ = new double *[N];
    baseStiff_ = new double *[N];
    for (int i = 0; i < N; i++) {
        baseMass_[i] = new double[N];
        baseStiff_[i] = new double[N];
    }

    baseMass_[0][0] = 2; baseMass_[0][1] = 1;
    baseMass_[1][0] = 1; baseMass_[1][1] = 2;

    baseStiff_[0][0] = 1; baseStiff_[0][1] = -1;
    baseStiff_[1][0] = -1; baseStiff_[1][1] = 1;
}

BarElement::~BarElement()
{
    int N = nSize_;
    for (int i = 0; i < N; i ++){
        delete [] baseMass_[i];
        delete [] baseStiff_[i];
    }
}
