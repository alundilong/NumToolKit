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

#include "ShearPanelElement42.h"

namespace NumToolKit {

namespace Fea {

const int ShearPanelElement42::nNode = 4;
const int ShearPanelElement42::nDOF = 2;

ShearPanelElement42::ShearPanelElement42()
    :
      FEAElementTwoD()
{
}

ShearPanelElement42::ShearPanelElement42
(
        const std::string & dimension,
        const std::string & name,
        const MaterialEle &m,
        const GeometryEle &g
) :
    FEAElementTwoD(dimension, name, m, g)
{
    f1_ = 0.0;
    f2_ = 0.0;
    infoAboutThisElement();
    numberSequence();
    constructGeometry();
    constructBaseMatrix();
    transformToGlobal();
}

void ShearPanelElement42::infoAboutThisElement()
{
    //  For in-plane deformation of shear panels (rectangular, 4node-2dof)
    log_ = "ShearPanel 42 Element is selected \n";
    nDOFEle_ = ShearPanelElement42::nDOF;
    nNodeEle_ = ShearPanelElement42::nNode;
    name_ = "ClassicPlate43";
    log_ += QString(name_+" is %1D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());
    qDebug() << log_;
}

void ShearPanelElement42::constructGeometry()
{
    const QList<QVector3D> &points = geometry()->twoDMesh().points();
    QVector3D p1 = points[pointIds()[1-1]];
    QVector3D p2 = points[pointIds()[2-1]];
    QVector3D p3 = points[pointIds()[3-1]];
    QVector3D p4 = points[pointIds()[4-1]];

    // set length in each direction
    double ex = p1.distanceToPoint(p2);
    double ey = p1.distanceToPoint(p4);
    double ez = geometry()->twoDMesh().thickness();
    area() = ex*ey;
    exyz() = {ex, ey, ez};
    const QVector3D& origin = geometry()->center();
    QVector3D axisX(((p2+p3)/2-origin).normalized());
    QVector3D axisY(((p4+p3)/2-origin).normalized());
//    qDebug() << "-------------------" << geometry()->twoDMesh().direction();
    lcs().setOXYZ(origin, axisX, axisY, geometry()->twoDMesh().direction());
}

void ShearPanelElement42::constructBaseMatrix()
{
    const int N = nNode*nDOF; // 4*2

    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

    const MaterialEle & mat = *material();
//    const GeometryEle & g = *geometry();

    const double & E1 = mat.E();
    const double & v = mat.nu();
    const double & rho = mat.rho();
    const double & h = exyz().ez;
    const double m = rho*h; // m = mass/area
//    const double D = E*h*h*h/12.0/(1.0-nu*nu);
    const double & a = exyz().ex;
    const double & b = exyz().ey;

    baseStiff_[0][0] = h*((E1*b)/(3*a) - (E1*a*(v - 1))/(6*b));
    baseStiff_[0][1] = (E1*h*(v + 1))/8;
    baseStiff_[0][2] = -h*((E1*b)/(3*a) + (E1*a*(v - 1))/(12*b));
    baseStiff_[0][3] = (E1*h*(3*v - 1))/8;
    baseStiff_[0][4] = -h*((E1*b)/(6*a) - (E1*a*(v - 1))/(12*b));
    baseStiff_[0][5] =  -(E1*h*(v + 1))/8;
    baseStiff_[0][6] = h*((E1*b)/(6*a) + (E1*a*(v - 1))/(6*b));
    baseStiff_[0][7] = -(E1*h*(3*v - 1))/8;

    baseStiff_[1][0] = (E1*h*(v + 1))/8;
    baseStiff_[1][1] = h*((E1*a)/(3*b) - (E1*b*(v - 1))/(6*a));
    baseStiff_[1][2] = -(E1*h*(3*v - 1))/8;
    baseStiff_[1][3] = h*((E1*a)/(6*b) + (E1*b*(v - 1))/(6*a));
    baseStiff_[1][4] = -(E1*h*(v + 1))/8;
    baseStiff_[1][5] = -h*((E1*a)/(6*b) - (E1*b*(v - 1))/(12*a));
    baseStiff_[1][6] = (E1*h*(3*v - 1))/8;
    baseStiff_[1][7] = -h*((E1*a)/(3*b) + (E1*b*(v - 1))/(12*a));

    baseStiff_[2][0] = -h*((E1*b)/(3*a) + (E1*a*(v - 1))/(12*b));
    baseStiff_[2][1] = -(E1*h*(3*v - 1))/8;
    baseStiff_[2][2] = h*((E1*b)/(3*a) - (E1*a*(v - 1))/(6*b));
    baseStiff_[2][3] = -(E1*h*(v + 1))/8;
    baseStiff_[2][4] = h*((E1*b)/(6*a) + (E1*a*(v - 1))/(6*b));
    baseStiff_[2][5] = (E1*h*(3*v - 1))/8;
    baseStiff_[2][6] = -h*((E1*b)/(6*a) - (E1*a*(v - 1))/(12*b));
    baseStiff_[2][7] = (E1*h*(v + 1))/8;

    baseStiff_[3][0] = (E1*h*(3*v - 1))/8;
    baseStiff_[3][1] = h*((E1*a)/(6*b) + (E1*b*(v - 1))/(6*a));
    baseStiff_[3][2] = -(E1*h*(v + 1))/8;
    baseStiff_[3][3] = h*((E1*a)/(3*b) - (E1*b*(v - 1))/(6*a));
    baseStiff_[3][4] = -(E1*h*(3*v - 1))/8;
    baseStiff_[3][5] = -h*((E1*a)/(3*b) + (E1*b*(v - 1))/(12*a));
    baseStiff_[3][6] = (E1*h*(v + 1))/8;
    baseStiff_[3][7] = -h*((E1*a)/(6*b) - (E1*b*(v - 1))/(12*a));

    baseStiff_[4][0] =  -h*((E1*b)/(6*a) - (E1*a*(v - 1))/(12*b));
    baseStiff_[4][1] = -(E1*h*(v + 1))/8;
    baseStiff_[4][2] = h*((E1*b)/(6*a) + (E1*a*(v - 1))/(6*b));
    baseStiff_[4][3] = -(E1*h*(3*v - 1))/8;
    baseStiff_[4][4] = h*((E1*b)/(3*a) - (E1*a*(v - 1))/(6*b));
    baseStiff_[4][5] = (E1*h*(v + 1))/8;
    baseStiff_[4][6] = -h*((E1*b)/(3*a) + (E1*a*(v - 1))/(12*b));
    baseStiff_[4][7] = (E1*h*(3*v - 1))/8;

    baseStiff_[5][0] = -(E1*h*(v + 1))/8;
    baseStiff_[5][1] = -h*((E1*a)/(6*b) - (E1*b*(v - 1))/(12*a));
    baseStiff_[5][2] = (E1*h*(3*v - 1))/8;
    baseStiff_[5][3] = -h*((E1*a)/(3*b) + (E1*b*(v - 1))/(12*a));
    baseStiff_[5][4] = (E1*h*(v + 1))/8;
    baseStiff_[5][5] = h*((E1*a)/(3*b) - (E1*b*(v - 1))/(6*a));
    baseStiff_[5][6] = -(E1*h*(3*v - 1))/8;
    baseStiff_[5][7] = h*((E1*a)/(6*b) + (E1*b*(v - 1))/(6*a));

    baseStiff_[6][0] = h*((E1*b)/(6*a) + (E1*a*(v - 1))/(6*b));
    baseStiff_[6][1] = (E1*h*(3*v - 1))/8;
    baseStiff_[6][2] = -h*((E1*b)/(6*a) - (E1*a*(v - 1))/(12*b));
    baseStiff_[6][3] = (E1*h*(v + 1))/8;
    baseStiff_[6][4] = -h*((E1*b)/(3*a) + (E1*a*(v - 1))/(12*b));
    baseStiff_[6][5] = -(E1*h*(3*v - 1))/8;
    baseStiff_[6][6] = h*((E1*b)/(3*a) - (E1*a*(v - 1))/(6*b));
    baseStiff_[6][7] = -(E1*h*(v + 1))/8;

    baseStiff_[7][0] = -(E1*h*(3*v - 1))/8;
    baseStiff_[7][1] = -h*((E1*a)/(3*b) + (E1*b*(v - 1))/(12*a));
    baseStiff_[7][2] = (E1*h*(v + 1))/8;
    baseStiff_[7][3] = -h*((E1*a)/(6*b) - (E1*b*(v - 1))/(12*a));
    baseStiff_[7][4] = (E1*h*(3*v - 1))/8;
    baseStiff_[7][5] = h*((E1*a)/(6*b) + (E1*b*(v - 1))/(6*a));
    baseStiff_[7][6] = -(E1*h*(v + 1))/8;
    baseStiff_[7][7] = h*((E1*a)/(3*b) - (E1*b*(v - 1))/(6*a));

    //baseStiff_.symmetry();

    // define baseMass matrix

    const double abm9 = a*b*m/9.0;
    const double abm18 = a*b*m/18.0;
    const double abm36 = a*b*m/36.0;

    baseMass_[0][0] = abm9;
    baseMass_[0][2] = abm18;
    baseMass_[0][4] = abm36;
    baseMass_[0][6] = abm18;

    baseMass_[1][1] = abm9;
    baseMass_[1][3] = abm18;
    baseMass_[1][5] = abm36;
    baseMass_[1][7] = abm18;

    baseMass_[2][2] = abm9;
    baseMass_[2][4] = abm18;
    baseMass_[2][6] = abm36;

    baseMass_[3][3] = abm9;
    baseMass_[3][5] = abm18;
    baseMass_[3][7] = abm36;

    baseMass_[4][4] = abm9;
    baseMass_[4][6] = abm18;

    baseMass_[5][5] = abm9;
    baseMass_[5][7] = abm18;

    baseMass_[6][6] = abm9;

    baseMass_[7][7] = abm9;

    baseMass_.symmetry();

}

void ShearPanelElement42::transformToGlobal()
{
    const QVector3D & e0 = lcs().e0();
    const QVector3D & e1 = lcs().e1();
    const QVector3D & e2 = lcs().e2();
//    qDebug() << e0 << e1 << e2;

    const QVector3D eg0(1.0,0.0,0.0);
    const QVector3D eg1(0.0,1.0,0.0);
    const QVector3D eg2(0.0,0.0,1.0);

    const int nrow = nDOF*nNode;
    mathExtension::Matrix G(nrow,nrow);
    int step = nDOF;
    for (int i = 0; i < nrow; i = i+step) {
        mathExtension::pos rowRange = {i,1,i+step};
        mathExtension::pos colRange = {i,1,i+step};
        mathExtension::Matrix T(nDOF,nDOF);
        //Transformation matrix
        T[0][0] = QVector3D::dotProduct(eg0, e0);
        T[0][1] = QVector3D::dotProduct(eg0, e1);
        T[0][2] = QVector3D::dotProduct(eg0, e2);
        T[1][0] = QVector3D::dotProduct(eg1, e0);
        T[1][1] = QVector3D::dotProduct(eg1, e1);
        T[1][2] = QVector3D::dotProduct(eg1, e2);
        T[2][0] = QVector3D::dotProduct(eg2, e0);
        T[2][1] = QVector3D::dotProduct(eg2, e1);
        T[2][2] = QVector3D::dotProduct(eg2, e2);

        G.setSubMatrix(rowRange, colRange, T, false);
    }

//    std::cout << G << std::endl;
    baseStiff_ = G.transpose()*baseStiff_*G;
    baseMass_ = G.transpose()*baseMass_*G;
}

void ShearPanelElement42::numberSequence()
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

    const QList<QVector3D> & points = geometry()->twoDMesh().points();

    double xc, yc, zc; // plate center
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

//    qDebug() << "XXXX";
//    qDebug() << vertex;
    pointIds() = vertex;
}

makeElement(ElementName, ShearPanelElement42, FEAElementTwoD, ShearPanelElement42)
makeElementNode(ElementName, ShearPanelElement42, FEAElementBase)
makeElementNodeDOF(ElementName, ShearPanelElement42, FEAElementBase)

} // end of Fea

} // end of NumToolKit


