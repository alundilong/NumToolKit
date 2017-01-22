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

#include "FeaElementTwoD.h"

namespace NumToolKit {

namespace Fea {

//const std::string FEAElementTwoD::typeName = "TwoD";

FEAElementTwoD::FEAElementTwoD()
    :
      FEAElementBase()
{
}

FEAElementTwoD::FEAElementTwoD\
(\
        const std::string &dimension, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g\
)
    :
      FEAElementBase(dimension, name, m, g)
{

}

std::unique_ptr<FEAElementTwoD> FEAElementTwoD::New
(
        const std::string &dimension,
        const std::string &name,
        const MaterialEle &m,
        const GeometryEle &g
)
{
    typename ElementNameConstructorTable::iterator cstrIter =
    ElementNameConstructorTablePtr_->find(name);

    return std::unique_ptr<FEAElementTwoD>
    (
        ((cstrIter->second))(dimension, name, m, g)
    );
}


//FEAElementBase::addSpaceDimensionConstructorToTable<FEAElementTwoD>
//addFEAElementTwoDSpaceDimensionConstructorToFEAElementBaseTable_;

makeElement(SpaceDimension, FEAElementTwoD, FEAElementBase, TwoD)
defineRunTimeSelectionTable(FEAElementTwoD, ElementName)

}

}
