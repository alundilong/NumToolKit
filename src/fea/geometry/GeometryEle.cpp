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

}

}