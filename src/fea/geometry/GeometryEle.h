/* -*- c++ -*- ----------------------------------------------------------
    NumToolKit - Numerical Simulation ToolKit
    Yijin Mao, ymao.mu@gmail.com

    This file is part of NumToolKit.

    NumToolKit is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NumToolKit is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NumToolKit.  If not, see <http://www.gnu.org/licenses/>.

    This software is distributed under
    the GNU General Public License.

    See the README file in the top-level NumToolKit directory.
------------------------------------------------------------------------- */

#ifndef GEOMETRYELE_H
#define GEOMETRYELE_H

#include "../../coordinateSystem/CoordSystem.h"
#include <QList>

namespace NumToolKit {

namespace Fea {

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

//  each element should be able to refer it
    const coordSystem * localCoordinateSystem() const { \
        return localCoordinateSystem_;\
    }

    const QList<int> pointIdList() const { return pointIdList_; }

private:
    double e_[3]; // length in x, y, z direction
    double A_;
    double volume_;

    QList<int> pointIdList_;

    coordSystem *localCoordinateSystem_;
};

}

}

#endif // GEOMETRYELE_H
