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

#ifndef EDGE_H
#define EDGE_H

#include <QString>
#include <QVector3D>
#include "Mesh.h"

namespace NumToolKit {

namespace Fea {

class Edge
{
public:
    Edge();
    Edge(const Mesh &mesh, const int & s, const int & e);
    inline const int & start() const { return s_; }
    inline const int & end() const { return e_; }
    inline const QString & edgeName() const { return edgeName_; }
    inline const QVector3D & direction() const { return direction_; }
    inline const QVector3D & center() const { return center_; }
    inline const double & length() const { return length_;}
private:
    int s_; // start edge
    int e_; // end edge
    QString edgeName_; // small index - large index
    QVector3D direction_;
    QVector3D center_;
    double length_;
};

}

}

#endif // EDGE_H
