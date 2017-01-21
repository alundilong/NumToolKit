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

#include "STLMesh.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

namespace NumToolKit {

namespace Fea {

STLMesh::STLMesh()
{
}

STLMesh::STLMesh(QString &fileName)
{
    nCells_ = 0;
    nFaces_ = 0;
    nNodes_ = 0;
    meshState_ = BAD;

    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(NULL,"..", fileName+" fail to open!");
        return;
    }

    QTextStream in(&file);
    in.skipWhiteSpace();
    in.readLine(); // skip the 1st line

    QVector3D normal;
    QVector3D p;
    QList<int> nodeList;
    int cFace = 0;
    int cNode = 0;
    while (!in.atEnd())
    {
       QString line = in.readLine().simplified(); //read one line at a time

       if(line.contains("normal")) {
           QStringList lstLine = line.split(" ");
//           qDebug() << lstLine;
           normal.setX(lstLine[2].toDouble());
           normal.setY(lstLine[3].toDouble());
           normal.setZ(lstLine[4].toDouble());
           faceNormals_.push_back(normal);
           cFace++;
           continue;
       }

       if(line.contains("loop") || \
               line.contains("endloop") || \
               line.contains("endfacet")) continue;

       if(line.contains("vertex")) {
           QStringList lstLine = line.split(" ");
//           qDebug() << lstLine;
           p.setX(lstLine[1].toDouble());
           p.setY(lstLine[2].toDouble());
           p.setZ(lstLine[3].toDouble());
           points_.push_back(p);
           nodeList.push_back(cNode);
           cNode++;
           if((cNode % 3) == 0 && (cFace > 1)) {
               faceNode_.push_back(nodeList);
               nodeList.clear();
           }

       }
    }

//    nodeList.push_back(cNode-1);

    nFaces_ = cFace;
    nNodes_ = cNode;
    nodeList.clear();
    nodeList.push_back(cNode-3);
    nodeList.push_back(cNode-2);
    nodeList.push_back(cNode-1);
    faceNode_.push_back(nodeList);
//    qDebug() << cFace << "\t" << cNode << "\t" << faceNode_.size() << "\t" << faceNormals_.size();

    file.close();

    meshState_ = GOOD;

    QList<double> px;
    QList<double> py;
    QList<double> pz;

    QList<QVector3D>::const_iterator it;
    for(it = points().begin(); it != points().end(); ++it) {
        px.push_back(((*it).x()));
        py.push_back(((*it).y()));
        pz.push_back(((*it).z()));
    }
    box_.xlow = *std::min_element(px.begin(),px.end());
    box_.xhig = *std::max_element(px.begin(),px.end());
    box_.ylow = *std::min_element(py.begin(),py.end());
    box_.yhig = *std::max_element(py.begin(),py.end());
    box_.zlow = *std::min_element(pz.begin(),pz.end());
    box_.zhig = *std::max_element(pz.begin(),pz.end());

//    cFace = 0;
//    QList<QList<int> >::const_iterator it;
//    for (it = faceNode().begin(); it != faceNode().end(); ++it) {
//        qDebug() << cFace << "\t" << faceNormals()[cFace] << "\t" << *it ;
//        cFace++;
//    }

}

const QList<int> STLMesh::faceNodes(int faceId) const
{
    return faceNode_[faceId];
}


const QList<int> STLMesh::cellFaces(int cellId) const
{
    return cellFace_[cellId];
}

}

}
