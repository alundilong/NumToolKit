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

#ifndef COORDSYSTEM_H
#define COORDSYSTEM_H

#include <QVector3D>
#include "../fea/math/MathExtension.h"
namespace NumToolKit {

namespace Fea{

class coordSystem
{

public:

    // find quadrant
    enum QuadrantCoord{PPN, NPN, NNN, PNN, PPP, NPP, NNP, PNP};
    enum Direction{X, Y, Z};
    static QuadrantCoord whichQuadrant(const mathExtension::Point &p);

    coordSystem();
    coordSystem(QVector3D, QVector3D, QVector3D, QVector3D);

    // accessible to each element's constructor
    inline const QVector3D  & e0() const { return e0_;}
    inline const QVector3D  & e1() const { return e1_;}
    inline const QVector3D  & e2() const { return e2_;}

    void setOXYZ(const QVector3D &o, const QVector3D &e0, const QVector3D &e1, const QVector3D &e2);

private:

    // must known members
    // 3 base vectors
    QVector3D e0_;
    QVector3D e1_;
    QVector3D e2_;

    // origin0 of coordinate system
    QVector3D origin0_;
};

}

}

#endif // COORDSYSTEM_H
