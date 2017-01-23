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

#ifndef GLOBALCOORDINATESYSTEM_H
#define GLOBALCOORDINATESYSTEM_H

#include <QVector3D>

namespace NumToolKit {

// global coordinate system
QVector3D eg0(1.0, 0.0, 0.0);
QVector3D eg1(0.0, 1.0, 0.0);
QVector3D eg2(0.0, 0.0, 1.0);

}


#endif // GLOBALCOORDINATESYSTEM_H
