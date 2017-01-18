#ifndef FEAELEMENTLINEARCUBICALELEMENT_H
#define FEAELEMENTLINEARCUBICALELEMENT_H

#include "feaelementthreed.h"

class FEAElementLinearCubicalElement : public FEAElementThreeD
{
public:
    static const std::string typeName;
    // element numbering sequence
    static const int  nNode;
    static const int nDOF;

public:
    FEAElementLinearCubicalElement();
    FEAElementLinearCubicalElement
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );
    ~FEAElementLinearCubicalElement();
};

#endif // FEAELEMENTLINEARCUBICALELEMENT_H
