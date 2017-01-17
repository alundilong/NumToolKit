#include "stringelement.h"

stringElement::stringElement()
{

}

stringElement::stringElement
(
        const int & dimension,
        const QString & name,
        const MaterialEle &m,
        const GeometryEle &g
):
    FEAElementBase(dimension, name, m, g)
{

}

stringElement::~stringElement()
{
    int N = nNodeEle_;
    for (int i = 0; i < N; i ++){
        delete [] baseMass_[i];
        delete [] baseStiff_[i];
    }
    delete [] baseMass_;
    delete [] baseStiff_;
}
