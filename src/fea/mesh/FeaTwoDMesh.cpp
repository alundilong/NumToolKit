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

#include "FeaTwoDMesh.h"
#include "../math/MathExtension.h"
#include <QDebug>

namespace NumToolKit {

namespace Fea {

FEATwoDMesh::FEATwoDMesh(const QVector3D & dir, const Mesh &mesh)
    :
    mesh_(mesh)
{
    // create edge list by looping over faces
    int size = mesh.owner().size();
    for (int faceI = 0; faceI < size; faceI++) {
        QList<int> nodes = mesh.faceNodes(faceI);
        QList<int>::const_iterator it = nodes.begin();
        int start, end;
        for(; it != nodes.end();) {
            start = *it;
            ++it;
            if(it == nodes.end()) end = *(nodes.begin());
            else end = *it;
            Edge e(mesh, start, end);
            if(QVector3D::dotProduct(e.direction(),dir) > 0) {
                nameMapEdge_[e.edgeName()] = e; // edgename = lowIndex-largeIndex
            }
        }
    }

    QMap<QString, Edge>::const_iterator nme;
    int c = 0;
    for (nme = nameMapEdge().begin(); nme != nameMapEdge().end(); ++nme) {
        const Edge &e = (*nme);
//        qDebug() << e.edgeName() << c;
        nameMapEdgeId_[e.edgeName()] = c; // labe of edge
        indexMapEdge_[c] = e;
        c++;
    }
    // total number of edge
    numEdge_ = nameMapEdge().size();// == nameMapEdge().uniqueKeys().size()

    // face edge
    for (int faceI = 0; faceI < size; faceI++) {

//        if (QVector3D::crossProduct(dir,mesh.faceNormals()[faceI]) == 0) continue;

        QList<int> nodes = mesh.faceNodes(faceI);
        QList<int>::const_iterator it = nodes.begin();
        int start, end;
        QList<int> fe;
        for(; it != nodes.end();) {
            start = *it;
            ++it;
            if(it == nodes.end()) end = *(nodes.begin());
            else end = *it;
            QString smallIndex = QString::number(mathExtension::imin(start, end));
            QString largeIndex = QString::number(mathExtension::imax(start, end));
            const QString name = smallIndex+"-"+largeIndex;
            if(nameMapEdge().contains(name)) {

                int index = nameMapEdgeId().value(name);
                fe.push_back(index);
//                qDebug() << name << index;

            }
        }
        faceEdges_.push_back(fe);
    }

    QList<QList<int> > cellEdges;
    size = mesh.nCells();

    for (int cellI = 0; cellI < size; cellI++) {
        const QList<int> &cellFace = mesh.cellFaces(cellI);
        QList<int>::const_iterator it;
        QList<int> ce;
        c = 0;
        for(it = cellFace.begin(); it != cellFace.end(); ++it) {
            ce.append(faceEdges()[*it]);
            c++;
        }
        cellEdges.push_back(ce);
    }

    // remove redundant edge label of cells
    QList<QList<int> >::const_iterator it;
    for (it = cellEdges.begin(); it != cellEdges.end(); ++it) {

        const QList<int> & edgeIds = *it;
        QList<int>::const_iterator it2;
        int num = edgeIds.size();
        int edgeId[num];
        int c = 0;
        for(it2 = edgeIds.begin(); it2 != edgeIds.end(); ++it2) {
            edgeId[c++] = *it2;
        }

        std::list<int> uniqueList(edgeId, edgeId+num);
        uniqueList.sort();
        uniqueList.unique();

        std::list<int>::const_iterator it3;
        QList<int> uniqueEdges;
        for (it3 = uniqueList.begin(); it3 != uniqueList.end(); ++it3) {
            uniqueEdges.push_back(*it3);
        }
        cellEdges_.push_back(uniqueEdges);
        elementNodes_.push_back(uniqueEdges);
    }

    createEdgeCenters();
    createBoundaryNameNodes();

}// end constructor

void FEATwoDMesh::createEdgeCenters()
{
    edgeCenters_.reserve(numEdge_);
    for (int i = 0; i < numEdge_; i++) {
        QVector3D q;
        edgeCenters_.append(q);
    }

    QMap<int, Edge>::const_iterator it;
    for (it = indexMapEdge().begin(); it != indexMapEdge().end(); ++it) {
        QVector3D center = (*it).center();
        edgeCenters_[it.key()] = center;
    }
}

void FEATwoDMesh::createBoundaryNameNodes()
{
    const QMap<QString, QList<int> > & mapBCFaceId = mesh_.boundaryNameFaces();
    // mesh() instead of mesh_ causes problem...
    QMap<QString, QList<int> >::const_iterator it;
    for (it = mapBCFaceId.begin(); it != mapBCFaceId.end(); ++it) {
        const QString & name = it.key();
        const QList<int> & faceIds = *it;
        QList<int> edgeIds;
        QList<int>::const_iterator itf;
        for (itf = faceIds.begin(); itf != faceIds.end(); ++itf) {
            edgeIds.append(faceEdges()[*itf]);
        }
        // remove redundant meb
        int num = edgeIds.size();
        int tmpEdgeIds[num];
        QList<int>::const_iterator it2;
        int c = 0;
        for(it2 = edgeIds.begin(); it2 != edgeIds.end(); ++it2) {
            tmpEdgeIds[c++] = *it2;
        }

        std::list<int> uniqueList(tmpEdgeIds, tmpEdgeIds+num);
        uniqueList.sort();
        uniqueList.unique();

        std::list<int>::const_iterator it3;
        edgeIds.clear();
        for (it3 = uniqueList.begin(); it3 != uniqueList.end(); ++it3) {
            edgeIds.push_back(*it3);
        }

        boundaryNameNodes_[name] = edgeIds;
    }
}

} // end fea

} // end NumToolKit
