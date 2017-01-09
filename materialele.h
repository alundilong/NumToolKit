#ifndef MATERIALELE_H
#define MATERIALELE_H


class MaterialEle
{

public:
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
