#ifndef EULERBERNOULLIBEAM_H
#define EULERBERNOULLIBEAM_H

#include "feaelementbase.h"

class EulerBernoulliBeam : public FEAElementBase
{
public:
    static const QString typeName;
    enum location {LEFT, RIGHT};
    static const int  nNode;
    static const int nDOF;
public:
    EulerBernoulliBeam();
    EulerBernoulliBeam(MaterialEle &m, GeometryEle &g);
    ~EulerBernoulliBeam();
};

#endif // EULERBERNOULLIBEAM_H
