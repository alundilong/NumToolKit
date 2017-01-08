#ifndef BARELEMENT_H
#define BARELEMENT_H

#include "feaelementbase.h"

class BarElement : public FEAElementBase
{
public:
    enum position {LEFT, RIGHT};
public:
    BarElement();
    ~BarElement();
};

#endif // BARELEMENT_H
