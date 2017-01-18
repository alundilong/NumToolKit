#include "materialele.h"

const std::unordered_map<std::string, MaterialOne> \
MaterialEle::materialType \
= \
{   /* Name     , {rho,   E,   G,   v}*/ \
    {"Aluminum", {2.7e3, 70e9, 26e9, 0.35} },\
    {"IronFe", {7.874e3, 211e9, 82e9, 0.29} },\
    {"StainlessSteelGrade304",{7.85e3,203e9,81e9,0.275} }
};

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
