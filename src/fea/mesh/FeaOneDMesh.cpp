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

#include "FeaOneDMesh.h"

namespace NumToolKit {

namespace Fea {

// dir is the direction of 1D element
FEAOneDMesh::FEAOneDMesh(const QVector3D & dir, const Mesh & mesh)
    :
    mesh_(mesh), dir_(dir)
{
    // find all faces whose normals are parallel to this direction
    QList<int>::const_iterator it;
    const QList<QVector3D> & faceNormals = mesh.faceNormals();
    int size = mesh.owner().size();
    int c = 0;
    for (int faceI = 0; faceI < size; faceI++) {
        const QVector3D & n = faceNormals[faceI];
        if(QVector3D::crossProduct(n,dir).length() == 0) {
            oneDNodes_.push_back(faceI);
            oneDNodeToThreeDFace_[c] = faceI;
            threeDFaceToOneDNode_[faceI] = c;
            c++;
        }
    }

    // find
    size = mesh.nCells();
    for (int cellI = 0; cellI < size; cellI++) {
        const QList<int> &faces = mesh.cellFaces(cellI);
        QList<int>::const_iterator it;
        QList<int> twoNodes;
        for (it = faces.begin(); it != faces.end(); ++it) {
            const int & faceI = *it;
            const QVector3D & n = faceNormals[faceI];
            if(QVector3D::crossProduct(n,dir).length() == 0) {
                twoNodes.push_back(threeDFaceToOneDNode_[faceI]);
            }
        }
        elementNodes_.push_back(twoNodes);
    }

    // compute face centers
    computeFaceCenters();
}

void FEAOneDMesh::createPoints()
{
    QList<QList<int> >::const_iterator it;
    const QList<double> & allFaceAreas = mesh().faceAreas();
    for(it = elementNodes().begin(); it != elementNodes().end(); ++it) {
        const int & first = (*it)[0];
        const int & second = (*it)[1];
        const int & faceI1 = oneDNodeToThreeDFace()[first];
        const int & faceI2 = oneDNodeToThreeDFace()[second];
        points_.push_back(faceCenters()[faceI1]);
        points_.push_back(faceCenters()[faceI2]);

        faceAreas_.push_back(allFaceAreas[faceI1]);
        faceAreas_.push_back(allFaceAreas[faceI2]);
    }
}

void FEAOneDMesh::computeFaceCenters()
{
    const QList<QVector3D> & points = mesh().points();
    // loop over all faces
    int faceId = 0;
    for (faceId = 0; faceId < mesh().nFaces(); faceId++) {
        // find nodeIds of faceId
        const QList<int> & nodeIds = mesh().faceNodes(faceId);
        QList<int>::const_iterator it;
        QVector3D center(0,0,0);
        for (it = nodeIds.begin(); it != nodeIds.end(); ++it) {
            center +=points[(*it)];
        }
        center /= nodeIds.size();
        faceCenters_.push_back(center);
    }
}

}// end of Fea

}// end of NumToolKit
