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

#include "BarElement.h"

namespace NumToolKit {

namespace Fea {

//const std::string BarElement::typeName = "Bar";
const int BarElement::nNode = 2;
const int BarElement::nDOF = 1;

BarElement::BarElement() : FEAElementOneD()
{
    log_ = "Bar Element is selected \n";
    dim_ = 1;
    nDOFEle_ = 1;
    nNodeEle_ = 2;
    name_ = "Bar";
    log_ += QString("%1D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());

    int N = nNodeEle()*nNodeEle();

    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

    baseMass_[0][0] = 2; baseMass_[0][1] = 1;
    baseMass_[1][0] = 1; baseMass_[1][1] = 2;

    baseStiff_[0][0] = 1; baseStiff_[0][1] = -1;
    baseStiff_[1][0] = -1; baseStiff_[1][1] = 1;

    // The material and geoinfo are null
    if (material() == NULL) {
        qDebug() << "Material for each Element are unknown!";
    }

    if (geometry() == NULL) {
        qDebug() << "GeoInfo for each Element are unknown!";
    }
}

BarElement::BarElement
(
        const std::string & dimension,
        const std::string & name,
        const MaterialEle &m,
        const GeometryEle &g
) :
    FEAElementOneD(dimension, name, m, g)
{
    infoAboutThisElement();
    numberSequence();
    constructGeometry();
    constructBaseMatrix();
    transformToGlobal();
}

BarElement::~BarElement()
{}

void BarElement::infoAboutThisElement()
{
    log_ = "OnD-Bar Element is selected \n";
//    dim_ = 1;
    nDOFEle_ = 1;
    nNodeEle_ = 2;
    name_ = "Bar";
    log_ += QString("%1D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());

}

void BarElement::constructGeometry()
{
    const QList<QVector3D> &points = geometry()->oneDMesh().points();
    QVector3D p1 = points[pointIds()[1-1]];
    QVector3D p2 = points[pointIds()[2-1]];

    const QList<double> & areas = geometry()->oneDMesh().faceAreas();
    area_[0] = areas[pointIds()[1-1]];
    area_[1] = areas[pointIds()[2-1]];

    // set length in each direction
    const double ex = p1.distanceToPoint(p2);
    // assume square
    const double ey = sqrt(area());
    const double ez = ey;
    volume_ = area()*ex;
    exyz() = {ex, ey, ez};
    const QVector3D& origin = geometry()->center();
    QVector3D axisX((p2-origin).normalized());
    lcs().setOXYZ(origin, axisX, axisX, axisX);

}

void BarElement::constructBaseMatrix()
{
    const int N = nNode*nDOF;

    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

    const MaterialEle & m = *material();
    const GeometryEle & g = *geometry();

//    const double *eL = g.e();
    const double ex = exyz().ex;
    const double mass = m.rho()*volume()/ex;
    const double coeffMass = mass*ex/6.0;

    baseMass_[0][0] = 2*coeffMass; baseMass_[0][1] = 1*coeffMass;
    baseMass_[1][0] = 1*coeffMass; baseMass_[1][1] = 2*coeffMass;

    const double E = m.E();;
    const double coeffStiff = E*area()/ex;

    baseStiff_[0][0] = 1*coeffStiff; baseStiff_[0][1] = -1*coeffStiff;
    baseStiff_[1][0] = -1*coeffStiff; baseStiff_[1][1] = 1*coeffStiff;

    // compute moment of inertia
    const double ey = exyz().ey;
    const double ez = exyz().ez;

    I[FEAElementBase::component::XX] = mass*(ey*ey+ez*ez)/12.;
    I[FEAElementBase::component::YY] = mass*(ex*ex+ez*ez)/12.;
    I[FEAElementBase::component::ZZ] = mass*(ex*ex+ey*ey)/12.;

}

void BarElement::transformToGlobal()
{
    const QVector3D & e0 = lcs().e0();
    const QVector3D eg0(1.0,0.0,0.0);

    const int nrow = nDOF*nNode;
    mathExtension::Matrix G(nrow,nrow);
    int step = BarElement::nDOF;
    for (int i = 0; i < nrow; i = i+step) {
        mathExtension::pos rowRange = {i,1,i+step};
        mathExtension::pos colRange = {i,1,i+step};
        mathExtension::Matrix T(nDOF,nDOF);
        //Transformation matrix
        T[0][0] = QVector3D::dotProduct(eg0, e0);

        G.setSubMatrix(rowRange, colRange, T, false);
    }

    baseStiff_ = G.transpose()*baseStiff_*G;
    baseMass_ = G.transpose()*baseMass_*G;

}

void BarElement::numberSequence()
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

    const QList<QVector3D> & points = geometry()->oneDMesh().points();

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
    vertex.reserve(2);
    for (int i = 0; i < 2; i++) {
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

        switch(coordSystem::whichQuadrant1D(vd)) {
        case coordSystem::N: {vertex[1-1] = vertexId; break;}
        case coordSystem::P: {vertex[2-1] = vertexId; break;}
        }
    }

    pointIds() = vertex;
}

makeElement(ElementName, BarElement, FEAElementOneD, Bar21)
makeElementNode(ElementName, BarElement, FEAElementBase)
makeElementNodeDOF(ElementName, BarElement, FEAElementBase)

//FEAElementOneD::addElementNameConstructorToTable<BarElement>
//addBarElementElementNameConstructorToFEAElementOneDTable_;

}

}


