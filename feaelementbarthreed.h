#ifndef FEAELEMENTBARTHREED_H
#define FEAELEMENTBARTHREED_H

#include "feaelementthreed.h"

class FEAElementBarThreeD : public FEAElementThreeD
{
public:
    static const std::string typeName;
    // element numbering sequence
    static const int  nNode;
    static const int nDOF;
public:
    FEAElementBarThreeD();
    FEAElementBarThreeD
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );
    ~FEAElementBarThreeD();
};

#endif // FEAELEMENTBARTHREED_H
