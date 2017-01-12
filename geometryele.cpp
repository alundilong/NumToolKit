#include "geometryele.h"

GeometryEle::GeometryEle()
{

}

GeometryEle::GeometryEle(double e[])
{
    for (int i = 0; i < 3; i++) {
        e_[i] = e[i];
    }

    double ex = e[component::X];
    double ey = e[component::Y];
    double ez = e[component::Z];

    A_ = ey*ez;
    volume_ = ex*ey*ez;
}

GeometryEle::GeometryEle(GeometryEle &g)
{
    e_[0] = g.e(component::X);
    e_[1] = g.e(component::Y);
    e_[2] = g.e(component::Z);
}

GeometryEle::~GeometryEle()
{

}

void GeometryEle::setLocalCoordinateSystem(coordSystem *cs)
{
    localCoordinateSystem_ = cs;
}
