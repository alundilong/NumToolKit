#include "stringelement.h"

stringElement::stringElement()
{

}

stringElement::stringElement(MaterialEle &m, GeometryEle &g)
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
