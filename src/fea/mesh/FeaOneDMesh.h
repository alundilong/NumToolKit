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

#ifndef FEAONEDMESH_H
#define FEAONEDMESH_H

#include "Mesh.h"
//#include "../../coordinateSystem/CoordSystem.h"
#include "../math/MathExtension.h"

namespace NumToolKit {

namespace Fea {

class FEAOneDMesh
{
public:
//    FEAOneDMesh();
    FEAOneDMesh(const QVector3D & dir, const Mesh& mesh);
    inline const QList<int> & oneDNodes() const { return oneDNodes_; }
    // use to 3D mesh coordinates
    inline const QMap<int, int> & oneDNodeToThreeDFace() const { return oneDNodeToThreeDFace_; }
    inline const QList<QList<int> > elementNodes() const { return elementNodes_; }
    // access to 3D mesh if necessary
    inline const Mesh & mesh() const { return mesh_; }
    inline const int & nCells() const { return nCells_; }
    inline const int & nNodes() const { return nNodes_; }
    inline const QList<QVector3D> points() const { return points_; }
    inline const QList<double> faceAreas() const { return faceAreas_; } // will be used for 1D bar/beam
    inline const QVector3D & direction() const { return dir_; }
    inline const QMap<QString, QList<int> > boundaryNameNodes() \
    const { return boundaryNameNodes_; }
    void fetchBCUniqueVertex(const QString &name, QList<int> &vertex) const;
    void dispTo3DMesh(const mathExtension::Vector &disp1d, mathExtension::Vector &disp3d) const;
private:
    QList<int> oneDNodes_; // all 3D faces contribute to 1D nodes
    QMap<int,int> oneDNodeToThreeDFace_;
    QMap<int,int> threeDFaceToOneDNode_;
    QList<QList<int> > elementNodes_;
    QList<QVector3D> points_;
    int nNodes_;
    int nCells_;
    const Mesh & mesh_;
    const QVector3D & dir_;
    QList<QVector3D> faceCenters_;
    QList<double> faceAreas_;

    void createPoints();
    void computeFaceCenters();
    inline const QList<QVector3D> faceCenters() const { return faceCenters_;}
    void createBoundaryNameNodes();
    QMap<QString, QList<int> > boundaryNameNodes_; //  a list of node

};


}
}


#endif // FEAONEDMESH_H
