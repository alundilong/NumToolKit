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
      vertexIds_(vertex),
      oneDMesh_(*static_cast<FEAOneDMesh*>(0)),
      twoDMesh_(*static_cast<FEATwoDMesh*>(0))
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
    center_ = QVector3D(cx, cy, cz);
}

GeometryEle::GeometryEle(const FEAOneDMesh &mesh, const QList<int> &vertex)
    :
      mesh_(*static_cast<Mesh*>(0)),
      vertexIds_(vertex),
      oneDMesh_(mesh),
      twoDMesh_(*static_cast<FEATwoDMesh*>(0))
{
    double cx, cy, cz;
    cx = cy = cz;
    int c = 0;
    QList<int>::const_iterator it;
    for (it = vertexIds().begin(); it != vertexIds().end(); ++it) {
        cx += oneDMesh_.points()[(*it)].x();
        cy += oneDMesh_.points()[(*it)].y();
        cz += oneDMesh_.points()[(*it)].z();
        c++;
    }
    cx = cx/c;
    cy = cy/c;
    cz = cz/c;
    center_ = QVector3D(cx, cy, cz);
}

GeometryEle::GeometryEle(const FEATwoDMesh &mesh, const QList<int> &vertex)
    :
      mesh_(*static_cast<Mesh*>(0)),
      vertexIds_(vertex),
      oneDMesh_(*static_cast<FEAOneDMesh*>(0)),
      twoDMesh_(mesh)
{
    double cx, cy, cz;
    cx = cy = cz;
    int c = 0;
    QList<int>::const_iterator it;
    for (it = vertexIds().begin(); it != vertexIds().end(); ++it) {
        cx += twoDMesh_.points()[(*it)].x();
        cy += twoDMesh_.points()[(*it)].y();
        cz += twoDMesh_.points()[(*it)].z();
        c++;
    }
    cx = cx/c;
    cy = cy/c;
    cz = cz/c;
    center_ = QVector3D(cx, cy, cz);
}

GeometryEle::GeometryEle(const GeometryEle &g)
    :
      mesh_(g.mesh()),
      vertexIds_(g.vertexIds()),
      oneDMesh_(g.oneDMesh()),
      twoDMesh_(g.twoDMesh())
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
    center_ = QVector3D(cx, cy, cz);
}

GeometryEle::~GeometryEle()
{

}

}

}
