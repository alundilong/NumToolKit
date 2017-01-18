#ifndef STRINGELEMENT_H
#define STRINGELEMENT_H

#include "feaelementoned.h"

class stringElement : public FEAElementOneD
{
public:
    static const std::string typeName;
    static const int  nNode;
    static const int nDOF;

    stringElement();
    stringElement
    (
            const std::string & dimension,
            const std::string & name,
            const MaterialEle &m,
            const GeometryEle &g
    );;
    ~stringElement();
};

#endif // STRINGELEMENT_H
