#ifndef BARELEMENT_H
#define BARELEMENT_H

#include "feaelementbase.h"

class BarElement : public FEAElementBase
{
public:
    static const std::string typeName;
    // element numbering sequence
    enum location {LEFT, RIGHT};
    static const int  nNode;
    static const int nDOF;
public:
    BarElement();
    BarElement
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle &m,
            const GeometryEle &g
    );
    ~BarElement();

};

#endif // BARELEMENT_H
