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


#ifndef FEATWODMESH_H
#define FEATWODMESH_H

#include "Mesh.h"
#include "Edge.h"

namespace NumToolKit {

namespace Fea {

class FEATwoDMesh
{
public:
    FEATwoDMesh(const QVector3D & dir, const Mesh & mesh);

    inline const QList<QList<int> > elementNodes() const { return elementNodes_; }
    // access to 3D mesh if necessary
    inline const Mesh & mesh() const { mesh_; }

private:
    const Mesh & mesh_;
    QList<QList<int> > faceEdges_;
    QList<QList<int> > cellEdges_;
    QMap<QString, Edge> nameMapEdge_;
    QMap<QString, int> nameMapEdgeId_;
    QMap<int, Edge> indexMapEdge_;

    QList<QList<int> > elementNodes_;
    QList<QVector3D> edgeCenters_;

    int numEdge_;

    const QMap<QString, Edge>  & nameMapEdge() const { return nameMapEdge_;}
    QMap<QString, int>  & nameMapEdgeId(){ return nameMapEdgeId_;}
    const QMap<int, Edge>  & indexMapEdge() const { return indexMapEdge_;}
    QList<QList<int> > & faceEdges() { return faceEdges_; }
    QList<QList<int> > & cellEdges() { return cellEdges_; }
    QList<QVector3D> & edgeCenters() { return edgeCenters_; }

    // create edge centers
    void createEdgeCenters();
};

}

}


#endif // FEATWODMESH_H
