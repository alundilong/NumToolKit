#include "stringelement.h"

//const std::string stringElement::typeName = "String";
const int stringElement::nNode = 2;
const int stringElement::nDOF = 2;

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
    const int N = nNode*nDOF;
    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);
}

stringElement::~stringElement()
{
//    int N = nNode*nDOF;
//    for (int i = 0; i < N; i ++){
//        delete [] baseMass_[i];
//        delete [] baseStiff_[i];
//    }
//    delete [] baseMass_;
//    delete [] baseStiff_;
}

makeElement(ElementName, stringElement, FEAElementOneD, String)

//FEAElementOneD::addElementNameConstructorToTable<stringElement>
//addstringElementElementNameConstructorToFEAElementOneDTable_;
