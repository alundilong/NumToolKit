#ifndef STRINGELEMENT_H
#define STRINGELEMENT_H
#include "feaelementbase.h"

class stringElement : public FEAElementBase
{
public:
    stringElement();
    stringElement(MaterialEle &m, GeometryEle &g);
    ~stringElement();
};

#endif // STRINGELEMENT_H
