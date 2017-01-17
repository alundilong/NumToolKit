#ifndef MATERIALELE_H
#define MATERIALELE_H

#include <string>
#include <unordered_map>
//#include "MaterialOne.h"

class MaterialEle
{

public:

//    static const std::unordered_map<std::string, MaterialOne> materiableLib[2];

    MaterialEle();
    MaterialEle(double rho, double E, double G);
    MaterialEle(MaterialEle &m);
    ~MaterialEle();

    const double & rho() const { return rho_; }
    const double & E() const { return E_; }
    const double & G() const { return G_; }

private:
    double rho_;
    double E_;
    double G_;

};

#endif // MATERIALELE_H
