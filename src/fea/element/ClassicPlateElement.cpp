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

#include "ClassicPlateElement.h"

namespace NumToolKit {

namespace Fea {

//const std::string BarElement::typeName = "Bar";
const int ClassicPlateElement::nNode = 4;
const int ClassicPlateElement::nDOF = 3;

ClassicPlateElement::ClassicPlateElement()
    :
      FEAElementTwoD()
{
}

ClassicPlateElement::ClassicPlateElement
(
        const std::string & dimension,
        const std::string & name,
        const MaterialEle &m,
        const GeometryEle &g
) :
    FEAElementTwoD(dimension, name, m, g)
{
    infoAboutThisElement();
    numberSequence();
    constructGeometry();
    constructBaseMatrix();
    transformToGlobal();
}

void ClassicPlateElement::infoAboutThisElement()
{
    // For transverse deflection of plates (rectangular, 4 node-3dof)
    log_ = "OnD-Bar Element is selected \n";
    nDOFEle_ = ClassicPlateElement::nDOF;
    nNodeEle_ = ClassicPlateElement::nNode;
    name_ = "ClassicPlate43";
    log_ += QString("%2D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());

}

void ClassicPlateElement::constructGeometry()
{
    const QList<QVector3D> &points = geometry()->mesh().points();
    QVector3D p1 = points[pointIds()[1-1]];
    QVector3D p2 = points[pointIds()[2-1]];
    QVector3D p3 = points[pointIds()[3-1]];
    QVector3D p4 = points[pointIds()[4-1]];

    // set length in each direction
    double ex = p1.distanceToPoint(p2);
    double ey = p1.distanceToPoint(p4);
    double ez = 0.0;
    exyz() = {ex, ey, ez};
    const QVector3D& origin = geometry()->center();
    QVector3D axisX((p2-origin).normalized());
    QVector3D axisY((p4-origin).normalized());
    lcs().setOXYZ(origin, axisX, axisY, axisX);
}

void ClassicPlateElement::constructBaseMatrix()
{
    const int N = nNode*nDOF;

    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

    const MaterialEle & m = *material();
    const GeometryEle & g = *geometry();
}

void ClassicPlateElement::transformToGlobal()
{
    const QVector3D & e0 = lcs().e0();
    const QVector3D & e1 = lcs().e1();
    const QVector3D eg0(1.0, 0.0, 0.0);
    const QVector3D eg1(0.0, 1.0, 0.0);

    mathExtension::Matrix G(4,4);
    int step = ClassicPlateElement::nDOF;
    for (int i = 0; i < 4; i = i+step) {
        mathExtension::pos rowRange = {i,1,i+step};
        mathExtension::pos colRange = {i,1,i+step};
        mathExtension::Matrix T(2,2);
        //Transformation matrix
        T[0][0] = QVector3D::dotProduct(eg0, e0);
        T[0][1] = QVector3D::dotProduct(eg0, e1);
        T[1][0] = QVector3D::dotProduct(eg1, e0);
        T[1][1] = QVector3D::dotProduct(eg1, e1);

        G.setSubMatrix(rowRange, colRange, T, false);
    }

    baseStiff_ = G.transpose()*baseStiff_*G;
    baseMass_ = G.transpose()*baseMass_*G;
}

void ClassicPlateElement::numberSequence()
{
    // for this element we know how to compute e
    const QList<int> &vertexIds = geometry()->vertexIds();
    int num = vertexIds.size();
    int nodeIds[num];
    for(int i = 0; i < num; i++) {
        nodeIds[i] = vertexIds[i];
    }
    std::list<int> uniqueList(nodeIds, nodeIds+num);
    uniqueList.sort();
    uniqueList.unique();

    const QList<QVector3D> & points = geometry()->mesh().points();

    double xc, yc, zc; // bar center
    xc = yc = zc = 0.0;
    int c = 0;
    std::list<int>::const_iterator it;
    for (it = uniqueList.begin(); it != uniqueList.end(); ++it) {

        xc += points[(*it)].x();
        yc += points[(*it)].y();
        zc += points[(*it)].z();
        c++;
    }

    xc = xc/c;
    yc = yc/c;
    zc = zc/c;

    center() = QVector3D(xc, yc, zc);

    mathExtension::Point vc(xc,yc,zc);

    c = 0;
    QList<int> vertex;
    vertex.reserve(4);
    for (int i = 0; i < 4; i++) {
        int ii;
        vertex.append(ii);
    }

    for (it = uniqueList.begin(); it != uniqueList.end(); ++it) {
        const int & vertexId = *it;

        double x = points[vertexId].x();
        double y = points[vertexId].y();
        double z = points[vertexId].z();

        mathExtension::Point v(x,y,z);
        mathExtension::Point vd(v-vc);

        switch(coordSystem::whichQuadrant2D(vd)) {
        case coordSystem::NN: {vertex[1-1] = vertexId; break;}
        case coordSystem::PN: {vertex[2-1] = vertexId; break;}
        case coordSystem::PP: {vertex[3-1] = vertexId; break;}
        case coordSystem::NP: {vertex[4-1] = vertexId; break;}
        }
    }

    pointIds() = vertex;
}

makeElement(ElementName, ClassicPlateElement, FEAElementTwoD, ClassicPlate43)
makeElementNode(ElementName, ClassicPlateElement, FEAElementBase)
makeElementNodeDOF(ElementName, ClassicPlateElement, FEAElementBase)

} // end of Fea

} // end NumToolKit


