#ifndef GEOMETRYELE_H
#define GEOMETRYELE_H

#include "coordsystem.h"

class GeometryEle
{
public:
    enum component {X, Y, Z};
public:
    GeometryEle();
    GeometryEle(double e[3]);
    GeometryEle(GeometryEle &g);
    ~GeometryEle();

    const double & e(component comp) const { return e_[comp]; }
    const double * e() const { return e_; }
    const double & A() const { return A_; }
    const double & volume() const { return volume_; }

    void setLocalCoordinateSystem(coordSystem *);

private:
    double e_[3]; // length in x, y, z direction
    double A_;
    double volume_;

    coordSystem *localCoordinateSystem_;
};

#endif // GEOMETRYELE_H
