#include "feaelementbase.h"
#include "materialele.h"
#include "geometryele.h"

FEAElementBase::FEAElementBase()
{
    material_ = nullptr;
    geoInfo_ = nullptr;
}

FEAElementBase::FEAElementBase
(
        const int & dim,
        const QString & name,
        const MaterialEle &m,
        const GeometryEle &g
)
{
    material_ = &m;
    geoInfo_ = &g;
}


FEAElementBase::~FEAElementBase()
{

}

//FEAElementBase::MaterialEle::MaterialEle()
//{

//}

//FEAElementBase::MaterialEle::MaterialEle(double rho, double E, double G)
//{
//    FEAElementBase::MaterialEle::rho_ = rho;
//    FEAElementBase::MaterialEle::E_ = E;
//    FEAElementBase::MaterialEle::G_ = G;
//}

//FEAElementBase::MaterialEle::MaterialEle(MaterialEle &m)
//{
//    FEAElementBase::MaterialEle::rho_ = m.rho();
//    FEAElementBase::MaterialEle::E_ = m.E();
//    FEAElementBase::MaterialEle::G_ = m.G();
//}

//FEAElementBase::MaterialEle::~MaterialEle()
//{

//}

//FEAElementBase::GeometryEle::GeometryEle()
//{

//}

//FEAElementBase::GeometryEle::GeometryEle(double e[3], double I[6])
//{
//    for (int i = 0; i < 3; i++) {
//        GeometryEle::e_[i] = e[i];
//    }
//    for (int i = 0; i < 6; i++) {
//        GeometryEle::I_[i] = I[i];
//    }
//}

//FEAElementBase::GeometryEle::GeometryEle(GeometryEle &g)
//{
//    FEAElementBase::GeometryEle::e_[0] = g.e(GeometryEle::X);
//    FEAElementBase::GeometryEle::e_[1] = g.e(GeometryEle::Y);
//    FEAElementBase::GeometryEle::e_[2] = g.e(GeometryEle::Z);
//}

//FEAElementBase::GeometryEle::~GeometryEle()
//{

//}
