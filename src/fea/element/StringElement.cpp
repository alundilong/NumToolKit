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

#include "StringElement.h"

namespace NumToolKit {

namespace Fea {

//const std::string stringElement::typeName = "String";
const int StringElement::nNode = 2;
const int StringElement::nDOF = 2;

StringElement::StringElement()
{

}

StringElement::StringElement
(
        const std::string & dimension,
        const std::string & name,
        const MaterialEle &m,
        const GeometryEle &g
):
    FEAElementOneD(dimension, name, m, g)
{
    infoAboutThisElement();
    numberSequence();
    constructGeometry();
    constructBaseMatrix();
    transformToGlobal();

}

StringElement::~StringElement()
{
}

void StringElement::infoAboutThisElement()
{

}

void StringElement::constructGeometry()
{

}

void StringElement::constructBaseMatrix()
{
    const int N = nNode*nDOF;
    baseMass_ = mathExtension::Matrix(N,N);
    baseStiff_ = mathExtension::Matrix(N,N);
}


void StringElement::transformToGlobal()
{

}

void StringElement::numberSequence()
{

}

makeElement(ElementName, StringElement, FEAElementOneD, String)
makeElementNode(ElementName, StringElement, FEAElementBase)
makeElementNodeDOF(ElementName, StringElement, FEAElementBase)
//FEAElementOneD::addElementNameConstructorToTable<stringElement>
//addstringElementElementNameConstructorToFEAElementOneDTable_;
}

}
