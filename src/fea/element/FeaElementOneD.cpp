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

#include "FeaElementOneD.h"

namespace NumToolKit {

namespace Fea {

//const std::string FEAElementOneD::typeName = "OneD";

FEAElementOneD::FEAElementOneD()
    :
      FEAElementBase()
{
    dim_ = 1;
}

FEAElementOneD::FEAElementOneD\
(\
        const std::string &dimension, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g\
)
    :
    FEAElementBase(dimension, name, m, g)
{
    dim_ = 1;
}

std::unique_ptr<FEAElementOneD> FEAElementOneD::New
(
        const std::string &dimension,
        const std::string &name,
        const MaterialEle &m,
        const GeometryEle &g
)
{
    typename ElementNameConstructorTable::iterator cstrIter =
    ElementNameConstructorTablePtr_->find(name);

    return std::unique_ptr<FEAElementOneD>
    (
        ((cstrIter->second))(dimension, name, m, g)
                );
}

void FEAElementOneD::infoAboutThisElement()
{
    std::cout << "No Implementation" << std::endl;
}

void FEAElementOneD::constructGeometry()
{
    std::cout << "No Implementation" << std::endl;
}

void FEAElementOneD::constructBaseMatrix()
{
    std::cout << "No Implementation" << std::endl;
}


void FEAElementOneD::transformToGlobal()
{
    std::cout << "No Implementation" << std::endl;
}

void FEAElementOneD::numberSequence(QList<int> &)
{
    std::cout << "No Implementation" << std::endl;
}


//FEAElementOneD::addSpaceDimensionConstructorToTable<FEAElementOneD>
//addFEAElementOneDSpaceDimensionConstructorToFEAElementBaseTable_;

makeElement(SpaceDimension, FEAElementOneD, FEAElementOneD, OneD)
defineRunTimeSelectionTable(FEAElementOneD, ElementName)

}

}
