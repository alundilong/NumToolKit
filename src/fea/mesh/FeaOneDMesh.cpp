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
    const QList<QVector3D> & faceNormals = mesh.faceNormals();
    int size = mesh.nFaces();
    int c = 0;
    for (int faceI = 0; faceI < size; faceI++) {
        const QVector3D & n = faceNormals[faceI];
        if(QVector3D::crossProduct(n,dir).length() == 0) {
//            qDebug() << n << dir ;
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
    // use facecenters as node coord
    createPoints();
    // create boundary nodes
    createBoundaryNameNodes();

    nNodes_ = elementNodes().size()+1;
    nCells_ = mesh_.nCells();
}

void FEAOneDMesh::fetchBCUniqueVertex(const QString &name, QList<int> &vertex) const
{
    QMap<QString, QList<int> >::const_iterator it;
    it = boundaryNameNodes().find(name);

    if(it != boundaryNameNodes().end()) {
        const QList<int> & nodeList = boundaryNameNodes()[name];
        QList<int>::const_iterator it2;
        for(it2 = nodeList.begin(); it2 != nodeList.end(); ++it2) {
            vertex.push_back(*it2);
        }

    } else {
        for (it = boundaryNameNodes().begin(); it != boundaryNameNodes().end(); ++it) {
            qDebug() << (*it).first();
        }
    }
}

void FEAOneDMesh::dispTo3DMesh(const mathExtension::Vector &disp1d, mathExtension::Vector &disp3d) const
{
    int c = 0;
    int size = nNodes_;
    const QList<QList<int> > & faceNodes = mesh().faceNode();
    for (c = 0; c < size; c++) {
        const int & faceI = oneDNodeToThreeDFace_[c];
        const QList<int> & nodeIds = faceNodes[faceI];
        QList<int>::const_iterator it;
        for(it = nodeIds.begin(); it != nodeIds.end(); ++it) {
            const int & nodeI = *it;
            disp3d[nodeI*3] = disp1d[3*c];
            disp3d[nodeI*3+1] = disp1d[3*c+1];
            disp3d[nodeI*3+2] = disp1d[3*c+2];
        }
    }
}

void FEAOneDMesh::createPoints()
{
//    QList<QList<int> >::const_iterator it;
//    const QList<double> & allFaceAreas = mesh().faceAreas();

//    for(it = elementNodes().begin(); it != elementNodes().end(); ++it) {
//        const int & first = (*it)[0];
//        const int & second = (*it)[1];

//        const int & faceI1 = oneDNodeToThreeDFace()[first];
//        const int & faceI2 = oneDNodeToThreeDFace()[second];
//        points_[first] = faceCenters()[faceI1];
//        points_[second] = faceCenters()[faceI2];

//        qDebug() << first << second << faceCenters()[faceI1] << faceCenters()[faceI2];

//        faceAreas_.push_back(allFaceAreas[faceI1]);
//        faceAreas_.push_back(allFaceAreas[faceI2]);
//    }

    // using oneDNodeToThreeDFace_ to assign coord and area for each node
    const QList<double> & allFaceAreas = mesh().faceAreas();
    QMap<int,int>::const_iterator it;
    for (it = oneDNodeToThreeDFace_.begin(); it != oneDNodeToThreeDFace_.end(); ++it) {
//        const int & oneDNodeId = it.key();
        const int & threeDFaceId  = it.value();
        points_.push_back(faceCenters()[threeDFaceId]);
        faceAreas_.push_back(allFaceAreas[threeDFaceId]);
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

void FEAOneDMesh::createBoundaryNameNodes()
{
    const QMap<QString, QList<int> > & mapBCFaceId = mesh_.boundaryNameFaces();

    QMap<QString, QList<int> >::const_iterator it;
    for (it = mapBCFaceId.begin(); it != mapBCFaceId.end(); ++it) {
        const QString & name = it.key();
        const QList<int> & faceIds = *it;

        QList<int>::const_iterator it2;
        QList<int> nodeIds;
        for(it2 = faceIds.begin(); it2 != faceIds.end(); ++it2) {
            const int & faceId = *it2;
            nodeIds.push_back(threeDFaceToOneDNode_[faceId]);
        }
        boundaryNameNodes_[name] = nodeIds;
    }
}

}// end of Fea

}// end of NumToolKit
