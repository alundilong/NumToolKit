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

#include "FeaElementThreeD.h"

namespace NumToolKit {

namespace Fea {

//const std::string FEAElementThreeD::typeName = "ThreeD";

FEAElementThreeD::FEAElementThreeD()
    :
      FEAElementBase()
{
    dim_ = 3;
}

FEAElementThreeD::FEAElementThreeD\
(\
        const std::string &dimension, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g\
)
    :
    FEAElementBase(dimension, name, m, g)
{
    dim_ = 3;
}

std::unique_ptr<FEAElementThreeD> FEAElementThreeD::New
(
        const std::string &dimension,
        const std::string &name,
        const MaterialEle &m,
        const GeometryEle &g
)
{
    typename ElementNameConstructorTable::iterator cstrIter =
    ElementNameConstructorTablePtr_->find(name);

    if(cstrIter == ElementNameConstructorTablePtr_->end()) {
        std::cout << "Error : Failure to find " \
                  << name \
                  << " Element " << std::endl;
        std::cout << "Valid Elements are :" << std::endl;

        for (cstrIter = ElementNameConstructorTablePtr_->begin(); \
             cstrIter != ElementNameConstructorTablePtr_->end();\
             ++cstrIter) {
            std::cout << cstrIter->first << " Elements " << std::endl;
        }
    }

    std::unique_ptr<FEAElementThreeD> ptr =
        (
            ((cstrIter->second))(dimension, name, m, g)
        );
    return ptr;
}

void FEAElementThreeD::infoAboutThisElement()
{
    std::cout << "No Implementation" << std::endl;
}

void FEAElementThreeD::constructGeometry()
{
    std::cout << "No Implementation" << std::endl;
}

void FEAElementThreeD::constructBaseMatrix()
{
    std::cout << "No Implementation" << std::endl;
}


void FEAElementThreeD::transformToGlobal()
{
    std::cout << "No Implementation" << std::endl;
}

void FEAElementThreeD::numberSequence(QList<int> &)
{
    std::cout << "No Implementation" << std::endl;
}


//FEAElementBase::addSpaceDimensionConstructorToTable<FEAElementThreeD>
//addFEAElementThreeDSpaceDimensionConstructorToFEAElementBaseTable_;

makeElement(SpaceDimension, FEAElementThreeD, FEAElementBase, ThreeD)
defineRunTimeSelectionTable(FEAElementThreeD, ElementName)

}

}
