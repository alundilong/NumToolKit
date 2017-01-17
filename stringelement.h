#ifndef STRINGELEMENT_H
#define STRINGELEMENT_H
#include "feaelementbase.h"

class stringElement : public FEAElementBase
{
public:
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
