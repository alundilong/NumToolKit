#include "materialele.h"

//const std::unordered_map<std::string, MaterialOne> MaterialEle::materiableLib[2] \
//= \
//{\
//    {"Aluminium", {7900., 1e9, 2e9, 0.3} },\
//    {"Iron", {8900., 1e9, 2e9, 0.3} }\
//};

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
