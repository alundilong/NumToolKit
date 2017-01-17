#include "stringelement.h"

stringElement::stringElement()
{

}

stringElement::stringElement
(
        const std::string & dimension,
        const std::string & name,
        const MaterialEle &m,
        const GeometryEle &g
):
    FEAElementOneD(dimension, name, m, g)
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

addElementToRunTimeSelectionTable(ElementName, stringElement, FEAElementOneD)

//FEAElementOneD::addElementNameConstructorToTable<stringElement>
//addstringElementElementNameConstructorToFEAElementOneDTable_;
