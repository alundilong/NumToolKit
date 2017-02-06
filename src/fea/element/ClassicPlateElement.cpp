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
    log_ = "Classic Plate 43 Element is selected \n";
    nDOFEle_ = ClassicPlateElement::nDOF;
    nNodeEle_ = ClassicPlateElement::nNode;
    name_ = "ClassicPlate43";
    log_ += QString(name_+" is %1D Element : nNode = %2 : DOF = %3 \n").arg(dim()).arg(nNodeEle()).arg(nDOFEle());
    qDebug() << log_;
}

void ClassicPlateElement::constructGeometry()
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
    QVector3D axisX((p2-origin).normalized());
    QVector3D axisY((p4-origin).normalized());
    lcs().setOXYZ(origin, axisX, axisY, geometry()->twoDMesh().direction());
}

void ClassicPlateElement::constructBaseMatrix()
{
    const int N = nNode*nDOF; // 4*3

    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);

    const MaterialEle & mat = *material();
    const GeometryEle & g = *geometry();

    const double & E = mat.E();
    const double & nu = mat.nu();
    const double & rho = mat.rho();
    const double & h = exyz().ez;
    const double m = rho*h; // m = mass/area
    const double D = E*h*h*h/12.0/(1.0-nu*nu);
    const double & a = exyz().ex;
    const double & b = exyz().ey;
    const double a2 = a*a; const double a4 = a2*a2; const double a3 = a2*a;
    const double b2 = b*b; const double b4 = b2*b2; const double b3 = b2*b;

    baseStiff_[0][0] = (2*D*(10*a4 + 7*a2*b2 + 10*b4))/(5*a3*b3); 
    baseStiff_[0][1] = (D*(10*a2 + b2))/(5*a*b2); 
    baseStiff_[0][2] =  -(D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[0][3] = -(2*D*(- 5*a4 + 7*a2*b2 + 10*b4))/(5*a3*b3);
    baseStiff_[0][4] = (D*a)/b2 - D/(5*a);
    baseStiff_[0][5] = -(D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[0][6] = -(2*D*(5*a4 - 7*a2*b2 + 5*b4))/(5*a3*b3);
    baseStiff_[0][7] = (D*a)/b2 - D/(5*a);
    baseStiff_[0][8] = (D*(a2 - 5*b2))/(5*a2*b);
    baseStiff_[0][9] = -(2*D*(10*a4 + 7*a2*b2 - 5*b4))/(5*a3*b3);
    baseStiff_[0][10] = (D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[0][11] = (D*(a2 - 5*b2))/(5*a2*b);

    baseStiff_[1][0] = (D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[1][1] = (4*D*(5*a2 + b2))/(15*a*b);
    baseStiff_[1][2] = 0;
    baseStiff_[1][3] = (D*a)/b2 - D/(5*a);
    baseStiff_[1][4] = (2*D*(5*a2 - 2*b2))/(15*a*b);
    baseStiff_[1][5] = 0;
    baseStiff_[1][6] = D/(5*a) - (D*a)/b2;
    baseStiff_[1][7] = (D*(5*a2 + b2))/(15*a*b);
    baseStiff_[1][8] = 0;
    baseStiff_[1][9] = -(D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[1][10] = (D*(10*a2 - b2))/(15*a*b);
    baseStiff_[1][11] = 0;

    baseStiff_[2][0] = -(D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[2][1] = 0;
    baseStiff_[2][2] = (4*D*(a2 + 5*b2))/(15*a*b);
    baseStiff_[2][3] = (D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[2][4] = 0;
    baseStiff_[2][5] = -(D*(a2 - 10*b2))/(15*a*b);
    baseStiff_[2][6] = (D*b)/a2 - D/(5*b);
    baseStiff_[2][7] = 0;
    baseStiff_[2][8] = (D*(a2 + 5*b2))/(15*a*b);
    baseStiff_[2][9] = (D*(a2 - 5*b2))/(5*a2*b);
    baseStiff_[2][10] = 0;
    baseStiff_[2][11] = -(2*D*(2*a2 - 5*b2))/(15*a*b);

    baseStiff_[3][0] = -(2*D*(- 5*a4 + 7*a2*b2 + 10*b4))/(5*a3*b3);
    baseStiff_[3][1] = (D*a)/b2 - D/(5*a);
    baseStiff_[3][2] = (D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[3][3] = (2*D*(10*a4 + 7*a2*b2 + 10*b4))/(5*a3*b3);
    baseStiff_[3][4] = (D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[3][5] = (D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[3][6] = -(2*D*(10*a4 + 7*a2*b2 - 5*b4))/(5*a3*b3);
    baseStiff_[3][7] = (D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[3][8] = (D*b)/a2 - D/(5*b);
    baseStiff_[3][9] = -(2*D*(5*a4 - 7*a2*b2 + 5*b4))/(5*a3*b3);
    baseStiff_[3][10] = (D*a)/b2 - D/(5*a);
    baseStiff_[3][11] = (D*b)/a2 - D/(5*b);

    baseStiff_[4][0] = (D*a)/b2 - D/(5*a);
    baseStiff_[4][1] = (2*D*(5*a2 - 2*b2))/(15*a*b);
    baseStiff_[4][2] = 0;
    baseStiff_[4][3] = (D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[4][4] = (4*D*(5*a2 + b2))/(15*a*b);
    baseStiff_[4][5] = 0;
    baseStiff_[4][6] = -(D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[4][7] = (D*(10*a2 - b2))/(15*a*b);
    baseStiff_[4][8] = 0;
    baseStiff_[4][9] = D/(5*a) - (D*a)/b2;
    baseStiff_[4][10] = (D*(5*a2 + b2))/(15*a*b);
    baseStiff_[4][11] = 0;

    baseStiff_[5][0]  = -(D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[5][1]  = 0; 
    baseStiff_[5][2]  = -(D*(a2 - 10*b2))/(15*a*b);
    baseStiff_[5][3] = (D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[5][4] = 0; 
    baseStiff_[5][5] = (4*D*(a2 + 5*b2))/(15*a*b);
    baseStiff_[5][6] = (D*b)/a2 - D/(5*b);
    baseStiff_[5][7] = 0; 
    baseStiff_[5][8] = -(2*D*(2*a2 - 5*b2))/(15*a*b);
    baseStiff_[5][9] = (D*(a2 - 5*b2))/(5*a2*b);
    baseStiff_[5][10] = 0; 
    baseStiff_[5][11] = (D*(a2 + 5*b2))/(15*a*b);

    baseStiff_[6][0]  = -(2*D*(5*a4 - 7*a2*b2 + 5*b4))/(5*a3*b3);
    baseStiff_[6][1]  = D/(5*a) - (D*a)/b2;
    baseStiff_[6][2]  = (D*b)/a2 - D/(5*b);
    baseStiff_[6][3]  = -(2*D*(10*a4 + 7*a2*b2 - 5*b4))/(5*a3*b3);
    baseStiff_[6][4]  = -(D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[6][5]  = (D*b)/a2 - D/(5*b);
    baseStiff_[6][6]  = (2*D*(10*a4 + 7*a2*b2 + 10*b4))/(5*a3*b3);
    baseStiff_[6][7]  =  -(D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[6][8]  = (D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[6][9]  = -(2*D*(- 5*a4 + 7*a2*b2 + 10*b4))/(5*a3*b3);
    baseStiff_[6][10] = D/(5*a) - (D*a)/b2;
    baseStiff_[6][11] = (D*(a2 + 10*b2))/(5*a2*b);

    baseStiff_[7][0]  = (D*a)/b2 - D/(5*a);
    baseStiff_[7][1]  = (D*(5*a2 + b2))/(15*a*b);
    baseStiff_[7][2]  = 0;
    baseStiff_[7][3]  = (D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[7][4]  = (D*(10*a2 - b2))/(15*a*b);
    baseStiff_[7][5]  = 0;
    baseStiff_[7][6]  = -(D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[7][7]  = (4*D*(5*a2 + b2))/(15*a*b);
    baseStiff_[7][8]  = 0;
    baseStiff_[7][9]  = D/(5*a) - (D*a)/b2;
    baseStiff_[7][10] = (2*D*(5*a2 - 2*b2))/(15*a*b);
    baseStiff_[7][11] = 0;

    baseStiff_[8][0]  = (D*(a2 - 5*b2))/(5*a2*b);
    baseStiff_[8][1]  = 0; 
    baseStiff_[8][2]  = (D*(a2 + 5*b2))/(15*a*b);
    baseStiff_[8][3]  = (D*b)/a2 - D/(5*b);
    baseStiff_[8][4]  = 0;
    baseStiff_[8][5]  = -(2*D*(2*a2 - 5*b2))/(1*a*b);
    baseStiff_[8][6]  = (D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[8][7]  = 0; 
    baseStiff_[8][8]  = (4*D*(a2 + 5*b2))/(15*a*b);
    baseStiff_[8][9]  = -(D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[8][10] = 0;
    baseStiff_[8][11] = -(D*(a2 - 10*b2))/(15*a*b);

    baseStiff_[9][0]  = -(2*D*(10*a4 + 7*a2*b2 - 5*b4))/(5*a3*b3);
    baseStiff_[9][1]  = -(D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[9][2]  = (D*(a2 - 5*b2))/(5*a2*b);
    baseStiff_[9][3]  = -(2*D*(5*a4 - 7*a2*b2 + 5*b4))/(5*a3*b3);
    baseStiff_[9][4]  = D/(5*a) - (D*a)/b2;
    baseStiff_[9][5]  = (D*(a2 - 5*b2))/(5*a2*b);
    baseStiff_[9][6]  = -(2*D*(- 5*a4 + 7*a2*b2 + 10*b4))/(5*a3*b3);
    baseStiff_[9][7]  = D/(5*a) - (D*a)/b2;
    baseStiff_[9][8]  = -(D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[9][9]  =  (2*D*(10*a4 + 7*a2*b2 + 10*b4))/(5*a3*b3);
    baseStiff_[9][10] = -(D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[9][11] = -(D*(a2 + 10*b2))/(5*a2*b);

    baseStiff_[10][0]  = (D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[10][1]  = (D*(10*a2 - b2))/(15*a*b);
    baseStiff_[10][2]  = 0;
    baseStiff_[10][3]  = (D*a)/b2 - D/(5*a);
    baseStiff_[10][4]  = (D*(5*a2 + b2))/(15*a*b);
    baseStiff_[10][5]  = 0;
    baseStiff_[10][6]  = D/(5*a) - (D*a)/b2;
    baseStiff_[10][7]  = (2*D*(5*a2 - 2*b2))/(15*a*b);
    baseStiff_[10][8]  = 0 ;
    baseStiff_[10][9]  = -(D*(10*a2 + b2))/(5*a*b2);
    baseStiff_[10][10] = (4*D*(5*a2 + b2))/(15*a*b);
    baseStiff_[10][11] = 0;

    baseStiff_[11][0]  = (D*(a2 - 5*b2))/(5*a2*b);
    baseStiff_[11][1]  =  0; 
    baseStiff_[11][2]  = -(2*D*(2*a2 - 5*b2))/(15*a*b);
    baseStiff_[11][3]  = (D*b)/a2 - D/(5*b);
    baseStiff_[11][4]  = 0; 
    baseStiff_[11][5]  = (D*(a2 + 5*b2))/(15*a*b);
    baseStiff_[11][6]  = (D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[11][7]  = 0; 
    baseStiff_[11][8]  = -(D*(a2 - 10*b2))/(15*a*b);
    baseStiff_[11][9]  = -(D*(a2 + 10*b2))/(5*a2*b);
    baseStiff_[11][10] = 0; 
    baseStiff_[11][11] = (4*D*(a2 + 5*b2))/(15*a*b);

    // define baseMass matrix
  
    baseMass_[0][0] = (1727*a*b*m)/12600; 
    baseMass_[0][1] = (461*a*b2*m)/25200;
    baseMass_[0][2] = -(461*a2*b*m)/25200;
    baseMass_[0][3] =  (613*a*b*m)/12600; 
    baseMass_[0][4] = (199*a*b2*m)/25200;
    baseMass_[0][5] = (137*a2*b*m)/12600;
    baseMass_[0][6] = (197*a*b*m)/12600; 
    baseMass_[0][7] =-(29*a*b2*m)/6300;
    baseMass_[0][8] = (29*a2*b*m)/6300;
    baseMass_[0][9] = (613*a*b*m)/12600; 
    baseMass_[0][10] = -(137*a*b2*m)/12600;
    baseMass_[0][11] = -(199*a2*b*m)/25200;

    baseMass_[1][0]  = (461*a*b2*m)/25200;
    baseMass_[1][1]  = (a*b3*m)/315;
    baseMass_[1][2]  = -(a2*b2*m)/400;
    baseMass_[1][3]  = (199*a*b2*m)/25200;
    baseMass_[1][4]  = (a*b3*m)/630;
    baseMass_[1][5]  = (a2*b2*m)/600;
    baseMass_[1][6]  = (29*a*b2*m)/6300;
    baseMass_[1][7]  = -(a*b3*m)/840;
    baseMass_[1][8]  = (a2*b2*m)/900;
    baseMass_[1][9]  = (137*a*b2*m)/12600;
    baseMass_[1][10] = -(a*b3*m)/420;
    baseMass_[1][11] = -(a2*b2*m)/600;

    baseMass_[2][0]  = -(461*a2*b*m)/25200;
    baseMass_[2][1]  =  -(a2*b2*m)/400;
    baseMass_[2][2]  =       (a3*b*m)/315;
    baseMass_[2][3]  = -(137*a2*b*m)/12600;
    baseMass_[2][4]  =  -(a2*b2*m)/600;
    baseMass_[2][5]  =    -(a3*b*m)/420;
    baseMass_[2][6]  =  -(29*a2*b*m)/6300;
    baseMass_[2][7]  =    (a2*b2*m)/900;
    baseMass_[2][8]  =     -(a3*b*m)/840;
    baseMass_[2][9]  = -(199*a2*b*m)/25200;
    baseMass_[2][10] =    (a2*b2*m)/600;
    baseMass_[2][11] =      (a3*b*m)/630;

    baseMass_[3][0]  =    (613*a*b*m)/12600; 
    baseMass_[3][1]  =  (199*a*b2*m)/25200;
    baseMass_[3][2]  =  -(137*a2*b*m)/12600;
    baseMass_[3][3]  =   (1727*a*b*m)/12600; 
    baseMass_[3][4]  =  (461*a*b2*m)/25200;
    baseMass_[3][5]  = (461*a2*b*m)/25200;
    baseMass_[3][6]  =    (613*a*b*m)/12600;
    baseMass_[3][7]  = -(137*a*b2*m)/12600;
    baseMass_[3][8]  =(199*a2*b*m)/25200;
    baseMass_[3][9]  =   (197*a*b*m)/12600; 
    baseMass_[3][10] =  -(29*a*b2*m)/6300;
    baseMass_[3][11] =  -(29*a2*b*m)/6300;
    
    baseMass_[4][0]  = (199*a*b2*m)/25200;
    baseMass_[4][1]  =  (a*b3*m)/630;
    baseMass_[4][2]  =   -(a2*b2*m)/600;
    baseMass_[4][3]  =   (461*a*b2*m)/25200;
    baseMass_[4][4]  =  (a*b3*m)/315;
    baseMass_[4][5]  =     (a2*b2*m)/400;
    baseMass_[4][6]  =  (137*a*b2*m)/12600;
    baseMass_[4][7]  =       -(a*b3*m)/420;
    baseMass_[4][8]  =    (a2*b2*m)/600;
    baseMass_[4][9]  =   (29*a*b2*m)/6300;
    baseMass_[4][10] =      -(a*b3*m)/840;
    baseMass_[4][11] =    -(a2*b2*m)/900;
    
    baseMass_[5][0]  = (137*a2*b*m)/12600;
    baseMass_[5][1]  =  (a2*b2*m)/600;
    baseMass_[5][2]  =    -(a3*b*m)/420;
    baseMass_[5][3]  =   (461*a2*b*m)/25200;
    baseMass_[5][4]  =   (a2*b2*m)/400;
    baseMass_[5][5]  =       (a3*b*m)/315;
    baseMass_[5][6]  =  (199*a2*b*m)/25200;
    baseMass_[5][7]  =     -(a2*b2*m)/600;
    baseMass_[5][8]  =      (a3*b*m)/630;
    baseMass_[5][9]  =   (29*a2*b*m)/6300;
    baseMass_[5][10] =    -(a2*b2*m)/900;
    baseMass_[5][11] =      -(a3*b*m)/840;
    
    baseMass_[6][0]  =  (197*a*b*m)/12600;   
    baseMass_[6][1]  =  (29*a*b2*m)/6300;
    baseMass_[6][2]  =  -(29*a2*b*m)/6300;
    baseMass_[6][3]  =    (613*a*b*m)/12600; 
    baseMass_[6][4]  =  (137*a*b2*m)/12600;
    baseMass_[6][5]  = (199*a2*b*m)/25200;
    baseMass_[6][6]  =   (1727*a*b*m)/12600;
    baseMass_[6][7]  = -(461*a*b2*m)/25200;
    baseMass_[6][8]  =(461*a2*b*m)/25200;
    baseMass_[6][9]  =   (613*a*b*m)/12600; 
    baseMass_[6][10] =-(199*a*b2*m)/25200;
    baseMass_[6][11] =-(137*a2*b*m)/12600;
    
    baseMass_[7][0]  = -(29*a*b2*m)/6300;
    baseMass_[7][1]  =   -(a*b3*m)/840;
    baseMass_[7][2]  =   (a2*b2*m)/900;
    baseMass_[7][3]  =  -(137*a*b2*m)/12600;
    baseMass_[7][4]  =    -(a*b3*m)/420;
    baseMass_[7][5]  =    -(a2*b2*m)/600;
    baseMass_[7][6]  = -(461*a*b2*m)/25200;
    baseMass_[7][7]  =        (a*b3*m)/315;
    baseMass_[7][8]  =   -(a2*b2*m)/400;
    baseMass_[7][9]  =-(199*a*b2*m)/25200;
    baseMass_[7][10] =       (a*b3*m)/630;
    baseMass_[7][11] =     (a2*b2*m)/600;
    
    baseMass_[8][0]  = (29*a2*b*m)/6300;
    baseMass_[8][1]  =  (a2*b2*m)/900;
    baseMass_[8][2]  =  -(a3*b*m)/840;
    baseMass_[8][3]  =  (199*a2*b*m)/25200;
    baseMass_[8][4]  =   (a2*b2*m)/600;
    baseMass_[8][5]  =       (a3*b*m)/630;
    baseMass_[8][6]  =  (461*a2*b*m)/25200;
    baseMass_[8][7]  =     -(a2*b2*m)/400;
    baseMass_[8][8]  =      (a3*b*m)/315;
    baseMass_[8][9]  = (137*a2*b*m)/12600;
    baseMass_[8][10] =    -(a2*b2*m)/600;
    baseMass_[8][11] =      -(a3*b*m)/420;
    
    baseMass_[9][0]  = (613*a*b*m)/12600;
    baseMass_[9][1]  =  (137*a*b2*m)/12600;
    baseMass_[9][2]  =  -(199*a2*b*m)/25200;
    baseMass_[9][3]  =   (197*a*b*m)/12600;
    baseMass_[9][4]  =  (29*a*b2*m)/6300;
    baseMass_[9][5]  =   (29*a2*b*m)/6300;
    baseMass_[9][6]  =    (613*a*b*m)/12600;
    baseMass_[9][7]  = -(199*a*b2*m)/25200;
    baseMass_[9][8]  =(137*a2*b*m)/12600;
    baseMass_[9][9]  =  (1727*a*b*m)/12600;
    baseMass_[9][10] =-(461*a*b2*m)/25200;
    baseMass_[9][11] =-(461*a2*b*m)/25200;
    
    baseMass_[10][0]  = -(137*a*b2*m)/12600;
    baseMass_[10][1]  =  -(a*b3*m)/420;
    baseMass_[10][2]  =    (a2*b2*m)/600;
    baseMass_[10][3]  =   -(29*a*b2*m)/6300;
    baseMass_[10][4]  =   -(a*b3*m)/840;
    baseMass_[10][5]  =    -(a2*b2*m)/900;
    baseMass_[10][6]  = -(199*a*b2*m)/25200;
    baseMass_[10][7]  =        (a*b3*m)/630;
    baseMass_[10][8]  =   -(a2*b2*m)/600;
    baseMass_[10][9]  =-(461*a*b2*m)/25200;
    baseMass_[10][10] =       (a*b3*m)/315;
    baseMass_[10][11] =     (a2*b2*m)/400;
    
    baseMass_[11][0]  = -(199*a2*b*m)/25200;
    baseMass_[11][1]  =  -(a2*b2*m)/600;
    baseMass_[11][2]  =   (a3*b*m)/630;
    baseMass_[11][3]  =   -(29*a2*b*m)/6300;
    baseMass_[11][4]  =  -(a2*b2*m)/900;
    baseMass_[11][5]  =      -(a3*b*m)/840;
    baseMass_[11][6]  = -(137*a2*b*m)/12600;
    baseMass_[11][7]  =      (a2*b2*m)/600;
    baseMass_[11][8]  =     -(a3*b*m)/420;
    baseMass_[11][9]  =-(461*a2*b*m)/25200;
    baseMass_[11][10] =     (a2*b2*m)/400;
    baseMass_[11][11] =       (a3*b*m)/315;
}

void ClassicPlateElement::transformToGlobal()
{
    const QVector3D & e0 = lcs().e0();
    const QVector3D & e1 = lcs().e1();
    const QVector3D & e2 = lcs().e2();

    const QVector3D eg0(1.0,0.0,0.0);
    const QVector3D eg1(0.0,1.0,0.0);
    const QVector3D eg2(0.0,0.0,1.0);

    mathExtension::Matrix G(12,12);
    int step = 3;
    for (int i = 0; i < 12; i = i+step) {
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

    pointIds() = vertex;
}

makeElement(ElementName, ClassicPlateElement, FEAElementTwoD, ClassicPlate43)
makeElementNode(ElementName, ClassicPlateElement, FEAElementBase)
makeElementNodeDOF(ElementName, ClassicPlateElement, FEAElementBase)

} // end of Fea

} // end NumToolKit


