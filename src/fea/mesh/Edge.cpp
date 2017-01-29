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

#include "Edge.h"
#include "../math/MathExtension.h"

namespace NumToolKit {

namespace Fea {

Edge::Edge()
{

}

Edge::Edge(const Mesh &mesh, const int &s, const int &e)
{
    const QList<QVector3D> & points = mesh.points();
    direction_ = (points[s] - points[e]).normalized();
    s_ = mathExtension::imin(s,e);
    e_ = mathExtension::imax(s,e);
    QString smallIndex = QString::number(s_);
    QString largeIndex = QString::number(e_);
    edgeName_ = smallIndex+"-"+largeIndex;
}

}

}

