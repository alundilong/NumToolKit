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

#ifndef STLMESH_H
#define STLMESH_H

#include <QString>
#include <QList>
#include <QVector3D>
#include "Bound.h"

/*
 * this would be merged with mesh class
*/

namespace NumToolKit {

namespace Fea {


class STLMesh
{
public:
    enum state {BAD = 0, GOOD = 1};
public:
    STLMesh();
    STLMesh(QString &fileName);

    const QList<int> faceNodes(int faceId) const; // return node
    const QList<int> cellNodes(int cellId) const; // return node
    const QList<int> cellFaces(int cellId) const; // return face

    // inline function
    inline int nCells() const { return nCells_; }
    inline int nFaces() const { return nFaces_; }
    inline int nNodes() const { return nNodes_; }
    inline bool meshState() const { return meshState_; }

    inline const bound box() const { return box_; }

    inline const QList< QList<int> > faceNode() const  { return faceNode_;}
    inline const QList< QList<int> > cellFace() const { return cellFace_;}

    inline const QList<QVector3D> points() const { return points_; }
    inline const QList<QVector3D> faceNormals() const { return faceNormals_; }

private:
    int nCells_;
    int nFaces_;
    int nNodes_;

    bool meshState_;

    QList< QList<int> > faceNode_; // find node from face
    QList< QList<int> > cellFace_; // find face from cell

    QList<QVector3D> points_; // list of point
    QList<QVector3D> faceNormals_; // normal of each face

    bound box_;
};

}

}

#endif // STLMESH_H
