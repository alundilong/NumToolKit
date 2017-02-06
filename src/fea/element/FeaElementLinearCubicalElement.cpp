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

#include "FeaElementLinearCubicalElement.h"

namespace NumToolKit {

namespace Fea {

const int FEAElementLinearCubicalElement::nNode = 8;
const int FEAElementLinearCubicalElement::nDOF = 3;

FEAElementLinearCubicalElement::FEAElementLinearCubicalElement()
{
}

FEAElementLinearCubicalElement::FEAElementLinearCubicalElement\
(\
        const std::string &dimension, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g
)
    : FEAElementThreeD(dimension, name, m, g)
{
    infoAboutThisElement();
    numberSequence();
    constructGeometry();
    constructBaseMatrix();
    transformToGlobal();
}

FEAElementLinearCubicalElement::~FEAElementLinearCubicalElement()
{
}

void FEAElementLinearCubicalElement::infoAboutThisElement()
{
    log_ = "3D LinearCubicalElement is selected \n";
    nDOFEle_ = FEAElementLinearCubicalElement::nDOF;
    nNodeEle_ = FEAElementLinearCubicalElement::nNode;
    name_ = "LinearCubicalElement3D";
    log_ += QString(\
                "%1D LinearCubicalElement : nNode = %2 : DOF = %3 \n"\
                ).arg(dim()).arg(nNodeEle()).arg(nDOFEle());
    qDebug() << log_;
}

void FEAElementLinearCubicalElement::numberSequence()
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

    double xc, yc, zc; // cube center
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
    vertex.reserve(8);
    for (int i = 0; i < 8; i++) {
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

        switch(coordSystem::whichQuadrant(vd)) {
        case coordSystem::PNP: {vertex[6-1] = vertexId; break;}
        case coordSystem::PPP: {vertex[7-1] = vertexId; break;}
        case coordSystem::NPP: {vertex[8-1] = vertexId; break;}
        case coordSystem::NNP: {vertex[5-1] = vertexId; break;}

        case coordSystem::NNN: {vertex[1-1] = vertexId; break;}
        case coordSystem::PNN: {vertex[2-1] = vertexId; break;}
        case coordSystem::PPN: {vertex[3-1] = vertexId; break;}
        case coordSystem::NPN: {vertex[4-1] = vertexId; break;}
        }
    }

    pointIds() = vertex;
}

void FEAElementLinearCubicalElement::constructGeometry()
{
    const QList<QVector3D> &points = geometry()->mesh().points();
    QVector3D p1 = points[pointIds()[1-1]];
    QVector3D p4 = points[pointIds()[4-1]];
    QVector3D p5 = points[pointIds()[5-1]];
    QVector3D p2 = points[pointIds()[2-1]];

    // set length in each direction
    double ex = p1.distanceToPoint(p2);
    double ey = p1.distanceToPoint(p4);
    double ez = p1.distanceToPoint(p5);
    exyz() = {ex, ey, ez};

    // set local coordinate system
    QVector3D p3 = points[pointIds()[3-1]];
    QVector3D p6 = points[pointIds()[6-1]];
    QVector3D p7 = points[pointIds()[7-1]];
    QVector3D p8 = points[pointIds()[8-1]];

    const QVector3D& origin = geometry()->center();

    QVector3D face2376 = (p2+p3+p7+p6)/4.0;
    QVector3D axisX((face2376-origin).normalized());

    QVector3D face4873 = (p4+p8+p7+p3)/4.0;
    QVector3D axisY((face4873-origin).normalized());

    QVector3D face5678 = (p5+p6+p7+p8)/4.0;
    QVector3D axisZ((face5678-origin).normalized());

    lcs().setOXYZ(origin, axisX, axisY, axisZ);

}

void FEAElementLinearCubicalElement::constructBaseMatrix()
{
    const int Nunknown = nNode*nDOF;
    baseMass_ = mathExtension::Matrix(Nunknown,Nunknown);
    baseStiff_ = mathExtension::Matrix(Nunknown,Nunknown);

    double a = exyz().ex;
    double b = exyz().ey;
    double c = exyz().ez;

    double v = material()->nu();
    double E = material()->E();
    double rho = material()->rho();

    double v22 = (1.0 - 2.0*v)/2.;
    double abc8 = a*b*c/8.0;

    mathExtension::Vector N(8);
    mathExtension::Vector Nx(8);
    mathExtension::Vector Ny(8);
    mathExtension::Vector Nz(8);

    double QQ[6][6] = \
    {
        {1-v,  v,    v,   0,   0,     0},\
        {v,    1-v,  v,   0,   0,     0},\
        {v,    v,    1-v, 0,   0,     0},\
        {0,    0,    0,   v22, 0,     0},\
        {0,    0,    0,   0,   v22,   0},\
        {0,    0,    0,   0,   0,   v22}\
    };

    for(int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            QQ[i][j] *= E/(1.0+v)/(1.0 - 2*v);
        }
    }

    mathExtension::Matrix Q(6,6);
    mathExtension::Vector V(6);
    for(int i = 0; i < Q.nrow(); i++) {
        for (int j = 0; j < Q.ncol(); j++) {
            V.set(j,QQ[i][j]);
        }
        Q.set(i,V);
    }

    // define stiff/mass matrix
    for (int i = 0; i < 2; i++) {
        long double x = mathExtension::gaussP(2, i);
        for (int j = 0; j < 2; j++) { //
            long double y = mathExtension::gaussP(2, j);
            for (int k = 0; k < 2; k++) {
                long double z = mathExtension::gaussP(2, k);

                long double wx = mathExtension::gaussW(2, i);
                long double wy = mathExtension::gaussW(2, j);
                long double wz = mathExtension::gaussW(2, k);
                long double weight = wx*wy*wz*abc8;
                long double xy = x*y; long double yz = y*z; \
                long double zx = z*x; long double xyz = x*y*z;

                N[0] = (1-x-y-z+xy+yz+zx-xyz)/8.0;
                N[1] = (1+x-y-z-xy+yz-zx+xyz)/8.0;
                N[2] = (1+x+y-z+xy-yz-zx-xyz)/8.0;
                N[3] = (1-x+y-z-xy-yz+zx+xyz)/8.0;
                N[4] = (1-x-y+z+xy-yz-zx+xyz)/8.0;
                N[5] = (1+x-y+z-xy-yz+zx-xyz)/8.0;
                N[6] = (1+x+y+z+xy+yz+zx+xyz)/8.0;
                N[7] = (1-x+y+z-xy+yz-zx-xyz)/8.0;

                Nx[0] = (-1+y+z-yz)/4.0/a;
                Nx[1] = (1-y-z+yz)/4.0/a;
                Nx[2] = (1+y-z-yz)/4.0/a;
                Nx[3] = (-1-y+z+yz)/4.0/a;
                Nx[4] = (-1+y-z+yz)/4.0/a;
                Nx[5] = (1-y+z-yz)/4.0/a;
                Nx[6] = (1+y+z+yz)/4.0/a;
                Nx[7] = (-1-y-z-yz)/4.0/a;

                Ny[0] = (-1+x+z-zx)/4.0/b;
                Ny[1] = (-1-x+z+zx)/4.0/b;
                Ny[2] = (1+x-z-zx)/4.0/b;
                Ny[3] = (1-x-z+zx)/4.0/b;
                Ny[4] = (-1+x-z+zx)/4.0/b;
                Ny[5] = (-1-x-z-zx)/4.0/b;
                Ny[6] = (1+x+z+zx)/4.0/b;
                Ny[7] = (1-x+z-zx)/4.0/b;

                Nz[0] = (-1+y+x-xy)/4.0/c;
                Nz[1] = (-1+y-x+xy)/4.0/c;
                Nz[2] = (-1-y-x-xy)/4.0/c;
                Nz[3] = (-1-y+x+xy)/4.0/c;
                Nz[4] = (1-y-x+xy)/4.0/c;
                Nz[5] = (1-y+x-xy)/4.0/c;
                Nz[6] = (1+y+x+xy)/4.0/c;
                Nz[7] = (1+y-x-xy)/4.0/c;

                mathExtension::Matrix N1(6,24);
                mathExtension::Matrix N2(3,24);

                mathExtension::pos colPos;
                colPos = {1, 3, 24};
                N1.setColValues(1, colPos, Nx, true);
                colPos = {2, 3, 24};
                N1.setColValues(2, colPos, Ny, true);
                colPos = {3, 3, 24};
                N1.setColValues(3, colPos, Nz, true);

                colPos = {2, 3, 24};
                N1.setColValues(4, colPos, Nz, true);
                colPos = {3, 3, 24};
                N1.setColValues(4, colPos, Ny, true);

                colPos = {1, 3, 24};
                N1.setColValues(5, colPos, Nz, true);
                colPos = {3, 3, 24};
                N1.setColValues(5, colPos, Nx, true);

                colPos = {1, 3, 24};
                N1.setColValues(6, colPos, Ny, true);
                colPos = {2, 3, 24};
                N1.setColValues(6, colPos, Nx, true);

                baseStiff_ = baseStiff_ + N1.transpose()*Q*(N1*weight);

                // prepare for mass matrix
                colPos = {1, 3, 24};
                N2.setColValues(1, colPos, N, true);
                colPos = {2, 3, 24};
                N2.setColValues(2, colPos, N, true);
                colPos = {3, 3, 24};
                N2.setColValues(3, colPos, N, true);

                // find mass matrix
                baseMass_ = baseMass_ + N2.transpose()*(N2*weight*rho);
            }
        }
    }

}


void FEAElementLinearCubicalElement::transformToGlobal()
{
    // coordinate system transformation
    // from local to global

    // could be further optimized since the transformation
    // matrix is exactly same to each node of the element

    const QVector3D & e0 = lcs().e0();
    const QVector3D & e1 = lcs().e1();
    const QVector3D & e2 = lcs().e2();

    const QVector3D eg0(1.0,0.0,0.0);
    const QVector3D eg1(0.0,1.0,0.0);
    const QVector3D eg2(0.0,0.0,1.0);

    mathExtension::Matrix G(24,24);
    int step = 3;
    for (int i = 0; i < 24; i = i+step) {
        mathExtension::pos rowRange = {i,1,i+step};
        mathExtension::pos colRange = {i,1,i+step};
        mathExtension::Matrix T(3,3);
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

//        std::cout << T << std::endl;

        G.setSubMatrix(rowRange, colRange, T, false);
    }

//    std::cout << G << std::endl;

    baseStiff_ = G.transpose()*baseStiff_*G;
    baseMass_ = G.transpose()*baseMass_*G;
}


makeElement(ElementName, \
            FEAElementLinearCubicalElement, \
            FEAElementThreeD, \
            LinearCubicalElementBeamThreeD)

makeElementNode(ElementName, FEAElementLinearCubicalElement, FEAElementBase)

makeElementNodeDOF(ElementName, FEAElementLinearCubicalElement, FEAElementBase)

}

}
