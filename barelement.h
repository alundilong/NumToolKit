#ifndef BARELEMENT_H
#define BARELEMENT_H

#include "feaelementbase.h"

class BarElement : public FEAElementBase
{
public:
    enum location {LEFT, RIGHT};
    static const int  nNode;
    static const int nDOF;
public:
    BarElement();
    BarElement(MaterialEle &m, GeometryEle &g);
    ~BarElement();

};

#endif // BARELEMENT_H
