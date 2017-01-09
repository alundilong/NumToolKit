#include "materialele.h"

MaterialEle::MaterialEle()
{

}

MaterialEle::MaterialEle(double rho, double E, double G)
{
    rho_ = rho;
    E_ = E;
    G_ = G;
}

MaterialEle::MaterialEle(MaterialEle &m)
{
    rho_ = m.rho();
    E_ = m.E();
    G_ = m.G();
}

MaterialEle::~MaterialEle()
{

}
