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

#include "GeometryEle.h"

namespace NumToolKit {

namespace Fea {

//GeometryEle::GeometryEle(const Mesh & mesh)
//    :
//      mesh_(mesh)
//{

//}

GeometryEle::GeometryEle(const Mesh &mesh, const QList<int> & vertex)
    :
      mesh_(mesh),
      vertexIds_(vertex)
{
    double cx, cy, cz;
    cx = cy = cz;
    int c = 0;
    QList<int>::const_iterator it;
    for (it = vertexIds().begin(); it != vertexIds().end(); ++it) {
        cx += mesh_.points()[(*it)].x();
        cy += mesh_.points()[(*it)].y();
        cz += mesh_.points()[(*it)].z();
        c++;
    }
    cx = cx/c;
    cy = cy/c;
    cz = cz/c;
    QVector3D origin(cx, cy, cz);
    QVector3D ex(1,0,0);
    QVector3D ey(0,1,0);
    QVector3D ez(0,0,1);
    localCoordinateSystem_ = new coordSystem(origin, ex, ey, ez);
}

//GeometryEle::GeometryEle(const Mesh & mesh, double e[])
//    :
//      mesh_(mesh)
//{
//    for (int i = 0; i < 3; i++) {
//        e_[i] = e[i];
//    }

//    double ex = e[component::X];
//    double ey = e[component::Y];
//    double ez = e[component::Z];

//    A_ = ey*ez;
//    volume_ = ex*ey*ez;
//}

GeometryEle::GeometryEle(const GeometryEle &g)
    :
      mesh_(g.mesh()),
      vertexIds_(g.vertexIds())
{
}

GeometryEle::~GeometryEle()
{

}

void GeometryEle::setLocalCoordinateSystem(coordSystem *cs)
{
    localCoordinateSystem_ = cs;
}

}

}
