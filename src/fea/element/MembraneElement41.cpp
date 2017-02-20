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

#include "MembraneElement41.h"

namespace NumToolKit {

namespace Fea {

const int MembraneElement41::nNode = 4;
const int MembraneElement41::nDOF = 1;

MembraneElement41::MembraneElement41()
    :
      FEAElementTwoD()
{
}

MembraneElement41::MembraneElement41
(
        const std::string & dimension,
        const std::string & name,
        const MaterialEle &m,
        const GeometryEle &g
) :
    FEAElementTwoD(dimension, name, m, g)
{
    t1_ = 0.0;
    t2_ = 0.0;
    infoAboutThisElement();
    numberSequence();
    constructGeometry();
    constructBaseMatrix();
    transformToGlobal();
}

void MembraneElement41::infoAboutThisElement()
{
    // For transverse deflection of membranes and temperature of 2D heat transfer (rectangular, 4node-1dof)
    log_ = "Membrane 41 Element is selected \n";
    nDOFEle_ = MembraneElement41::nDOF;
    nNodeEle_ = MembraneElement41::nNode;
    name_ = "ClassicPlate43";
    log_ += QString(name_+" is %1D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());
    qDebug() << log_;
}

void MembraneElement41::constructGeometry()
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

void MembraneElement41::constructBaseMatrix()
{
    const int N = nNode*nDOF; // 4*1

    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

    const MaterialEle & mat = *material();
//    const GeometryEle & g = *geometry();

    const double & E = mat.E();
    const double & nu = mat.nu();
    const double & rho = mat.rho();
    const double & h = exyz().ez;
    const double m = rho*h; // m = mass/area
    const double D = E*h*h*h/12.0/(1.0-nu*nu);
    const double & a = exyz().ex;
    const double & b = exyz().ey;

    const double & t1 = t1_;
    const double & t2 = t2_;
    const double at2_over_3b = a*t2/3.0/b; const double at2_over_6b = a*t2/6.0/b;
    const double bt1_over_3a = b*t1/3.0/a; const double bt1_over_6a = b*t1/6.0/a;

    baseStiff_[0][0] = at2_over_3b + bt1_over_3a;
    baseStiff_[0][1] = at2_over_6b - bt1_over_3a;
    baseStiff_[0][2] = -at2_over_6b - bt1_over_6a;
    baseStiff_[0][3] = -at2_over_3b + bt1_over_6a;

    baseStiff_[1][1] = at2_over_3b + bt1_over_3a;
    baseStiff_[1][2] = -at2_over_3b + bt1_over_6a;
    baseStiff_[1][3] = -at2_over_6b - bt1_over_6a;

    baseStiff_[2][2] = at2_over_3b + bt1_over_3a;
    baseStiff_[2][3] = at2_over_6b - bt1_over_3a;

    baseStiff_[3][3] = at2_over_3b + bt1_over_3a;
    baseStiff_.symmetry();

    // define baseMass matrix

    const double abm = a*b*m;
    baseMass_[0][0] = abm/9.;
    baseMass_[0][1] = abm/18.0;
    baseMass_[0][2] = abm/36.0;
    baseMass_[0][3] = abm/18.;

    baseMass_[1][1] = abm/9.;
    baseMass_[1][2] = abm/18.;
    baseMass_[1][3] = abm/36.;

    baseMass_[2][2] = abm/9.;
    baseMass_[2][3] = abm/18.;

    baseMass_[3][3] = abm/9.;
    baseMass_.symmetry();

}

void MembraneElement41::transformToGlobal()
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

void MembraneElement41::numberSequence()
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

makeElement(ElementName, MembraneElement41, FEAElementTwoD, MembraneElement41)
makeElementNode(ElementName, MembraneElement41, FEAElementBase)
makeElementNodeDOF(ElementName, MembraneElement41, FEAElementBase)

}// end of Fea

}// end of NumToolKit

