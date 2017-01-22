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
    constructGeometry();
    constructBaseMatrix();
    constructLocalCoordinateSystem();
    transformToGlocal();

    log_ = "3D LinearCubicalElement is selected \n";
    dim_ = 3;
    nDOFEle_ = 3;
    nNodeEle_ = 8;
    name_ = "LinearCubicalElement3D";
    log_ += QString(\
                "%1D LinearCubicalElement : nNode = %2 : DOF = %3 \n"\
                ).arg(dim()).arg(nNodeEle()).arg(nDOFEle());

    const int Nunknown = nNode*nDOF;

    baseMass_ = mathExtension::Matrix(Nunknown,Nunknown);
    baseStiff_ = mathExtension::Matrix(Nunknown,Nunknown);

    // for this element we know how to compute e

    QVector3D p1 = geometry()->mesh().points()[geometry()->vertexIds()[1-1]];
    QVector3D p4 = geometry()->mesh().points()[geometry()->vertexIds()[4-1]];
    QVector3D p5 = geometry()->mesh().points()[geometry()->vertexIds()[5-1]];
    QVector3D p2 = geometry()->mesh().points()[geometry()->vertexIds()[2-1]];

    double ex = p1.distanceToPoint(p4);
    double ey = p1.distanceToPoint(p5);
    double ez = p1.distanceToPoint(p2);
    double a = ex;
    double b = ey;
    double c = ez;

    double v = material()->nu(); // from material type
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
                long double z = mathExtension::gaussP(2, j);

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
                N1.setColValues(1, colPos, Nx);
                colPos = {2, 3, 24};
                N1.setColValues(2, colPos, Ny);
                colPos = {3, 3, 24};
                N1.setColValues(2, colPos, Nz);

                colPos = {2, 3, 24};
                N1.setColValues(4, colPos, Nz);
                colPos = {3, 3, 24};
                N1.setColValues(4, colPos, Ny);

                colPos = {1, 3, 24};
                N1.setColValues(5, colPos, Nz);
                colPos = {3, 3, 24};
                N1.setColValues(5, colPos, Nx);

                colPos = {1, 3, 24};
                N1.setColValues(6, colPos, Ny);
                colPos = {2, 3, 24};
                N1.setColValues(6, colPos, Nx);

                baseStiff_ = baseStiff_ + N1.transpose()*Q*N1*weight;

                // prepare for mass matrix
                colPos = {1, 3, 24};
                N2.setColValues(1, colPos, N);
                colPos = {2, 3, 24};
                N2.setColValues(2, colPos, N);
                colPos = {3, 3, 24};
                N2.setColValues(3, colPos, N);

                // find mass matrix
                baseMass_ = baseMass_ + N2.transpose()*(N2*weight*rho);
            }
        }
    }

    // coordinate system transformation
    // from local to global

    const coordSystem * cs = geometry()->localCoordinateSystem();
    mathExtension::Vector e0(3);
    mathExtension::Vector e1(3);
    mathExtension::Vector e2(3);
    double x = cs->e0().x();
    double y = cs->e0().y();
    double z = cs->e0().z();
    e0.set(0,x); e0.set(1,y); e0.set(2,z);
    x = cs->e1().x();
    y = cs->e1().y();
    z = cs->e1().z();
    e1.set(0,x); e1.set(1,y); e1.set(2,z);
    x = cs->e2().x();
    y = cs->e2().y();
    z = cs->e2().z();
    e2.set(0,x); e2.set(1,y); e2.set(2,z);

    mathExtension::Vector eg0(3);
    mathExtension::Vector eg1(3);
    mathExtension::Vector eg2(3);
    eg0.set(0,1.0);
    eg1.set(1,1.0);
    eg2.set(2,1.0);

    // could be further optimized since the transformation
    // matrix is exactly same to each node of the element

    mathExtension::Matrix G(24,24);
    int step = 3;
    for (int i = 0; i < 24; i = i+step) {
        mathExtension::pos rowRange = {i+1,1,i+1+step};
        mathExtension::pos colRange = {i+1,1,i+1+step};
        mathExtension::Matrix T(3,3);
        //Transformation matrix
        T[0][0] = eg0.cos(e0);//QVector3D::dotProduct(eg0, e0);
        T[0][1] = eg0.cos(e1);//QVector3D::dotProduct(eg0, e1);
        T[0][2] = eg0.cos(e2);//QVector3D::dotProduct(eg0, e2);
        T[1][0] = eg1.cos(e0);//QVector3D::dotProduct(eg1, e0);
        T[1][1] = eg1.cos(e1);//QVector3D::dotProduct(eg1, e1);
        T[1][2] = eg1.cos(e2);//QVector3D::dotProduct(eg1, e2);
        T[2][0] = eg2.cos(e0);//QVector3D::dotProduct(eg2, e0);
        T[2][1] = eg2.cos(e1);//QVector3D::dotProduct(eg2, e1);
        T[2][2] = eg2.cos(e2);//QVector3D::dotProduct(eg2, e2);

        G.setSubMatrix(rowRange, colRange, T);
    }

    baseStiff_ = G.transpose()*baseStiff_*G;
    baseMass_ = G.transpose()*baseMass_*G;
}

FEAElementLinearCubicalElement::~FEAElementLinearCubicalElement()
{
}

void FEAElementLinearCubicalElement::infoAboutThisElement()
{

}

void FEAElementLinearCubicalElement::constructGeometry()
{

}

void FEAElementLinearCubicalElement::constructBaseMatrix()
{

}

void FEAElementLinearCubicalElement::constructLocalCoordinateSystem()
{

}

void FEAElementLinearCubicalElement::transformToGlocal()
{

}

makeElement(ElementName, \
            FEAElementLinearCubicalElement, \
            FEAElementThreeD, \
            LinearCubicalElementBarThreeD)

makeElementNode(ElementName, FEAElementLinearCubicalElement, FEAElementBase)

makeElementNodeDOF(ElementName, FEAElementLinearCubicalElement, FEAElementBase)

}

}
