#include "stringelement.h"

//const std::string stringElement::typeName = "String";

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

makeElement(ElementName, stringElement, FEAElementOneD, String)

//FEAElementOneD::addElementNameConstructorToTable<stringElement>
//addstringElementElementNameConstructorToFEAElementOneDTable_;
