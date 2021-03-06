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
#include "../math/MathExtension.h"

namespace NumToolKit {

namespace Fea {

class FEATwoDMesh
{
public:
    FEATwoDMesh(const QVector3D & dir, const Mesh & mesh);

    inline const QList<QList<int> > elementNodes() const { return elementNodes_; }
    // access to 3D mesh if necessary
    inline const Mesh & mesh() const { return mesh_; }
//    inline const bound & box() const { return mesh_.box(); }

    inline const QMap<QString, QList<int> > boundaryNameNodes() \
    const { return boundaryNameNodes_; }

    inline const int & nNodes() const { return numEdge_; }
    inline const int & nCells() const { return nCells_;}
    inline const QVector3D & direction() const { return dir_; }
    const QList<QVector3D> & points() const { return edgeCenters_;}
    const double & thickness() const { return thickness_; }

    void fetchBCUniqueVertex(const QString &name, QList<int> &vertex) const;
    void dispTo3DMesh(const mathExtension::Vector &disp2d, mathExtension::Vector &disp3d) const;

private:
    const Mesh & mesh_;
    const QVector3D & dir_;
    QList<QList<int> > faceEdges_;
    QList<QList<int> > cellEdges_;
    QMap<QString, Edge> nameMapEdge_;
    QMap<QString, int> nameMapEdgeId_;
    QMap<int, Edge> indexMapEdge_;

    QList<QList<int> > elementNodes_;
    QList<QVector3D> edgeCenters_;
    QMap<QString, QList<int> > boundaryNameNodes_; //  a list of node

    int numEdge_;
    int nCells_;
    double thickness_;

    const QMap<QString, Edge>  & nameMapEdge() const { return nameMapEdge_;}
    QMap<QString, int>  & nameMapEdgeId(){ return nameMapEdgeId_;}
    const QMap<int, Edge>  & indexMapEdge() const { return indexMapEdge_;}
    QList<QList<int> > & faceEdges() { return faceEdges_; }
    QList<QList<int> > & cellEdges() { return cellEdges_; }
    QList<QVector3D> & edgeCenters() { return edgeCenters_; }


    // create edge centers
    void createEdgeCenters();
    void createBoundaryNameNodes();

};

}

}


#endif // FEATWODMESH_H
