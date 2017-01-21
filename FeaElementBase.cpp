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

#include "FeaElementBase.h"
#include "MaterialEle.h"
#include "GeometryEle.h"

namespace NumToolKit {

namespace Fea {

FEAElementBase::FEAElementBase()
{
    material_ = nullptr;
    geoInfo_ = nullptr;
}

FEAElementBase::FEAElementBase
(
        const std::string & dim,
        const std::string & name,
        const MaterialEle &m,
        const GeometryEle &g
)
{
    material_ = &m;
    geoInfo_ = &g;
}


FEAElementBase::~FEAElementBase()
{

}

std::auto_ptr<FEAElementBase> \
FEAElementBase::New\
(
        const std::string & dimension, \
        const std::string & name, \
        const MaterialEle & m, \
        const GeometryEle & g \
)
{
    typename SpaceDimensionConstructorTable::iterator cstrIter =
    SpaceDimensionConstructorTablePtr_->find(dimension);

    return std::auto_ptr<FEAElementBase>
    (
        (cstrIter->second)(dimension, name, m, g)
    );

}

defineRunTimeSelectionTable(FEAElementBase, SpaceDimension);

}

}
//FEAElementBase::MaterialEle::MaterialEle()
//{

//}

//FEAElementBase::MaterialEle::MaterialEle(double rho, double E, double G)
//{
//    FEAElementBase::MaterialEle::rho_ = rho;
//    FEAElementBase::MaterialEle::E_ = E;
//    FEAElementBase::MaterialEle::G_ = G;
//}

//FEAElementBase::MaterialEle::MaterialEle(MaterialEle &m)
//{
//    FEAElementBase::MaterialEle::rho_ = m.rho();
//    FEAElementBase::MaterialEle::E_ = m.E();
//    FEAElementBase::MaterialEle::G_ = m.G();
//}

//FEAElementBase::MaterialEle::~MaterialEle()
//{

//}

//FEAElementBase::GeometryEle::GeometryEle()
//{

//}

//FEAElementBase::GeometryEle::GeometryEle(double e[3], double I[6])
//{
//    for (int i = 0; i < 3; i++) {
//        GeometryEle::e_[i] = e[i];
//    }
//    for (int i = 0; i < 6; i++) {
//        GeometryEle::I_[i] = I[i];
//    }
//}

//FEAElementBase::GeometryEle::GeometryEle(GeometryEle &g)
//{
//    FEAElementBase::GeometryEle::e_[0] = g.e(GeometryEle::X);
//    FEAElementBase::GeometryEle::e_[1] = g.e(GeometryEle::Y);
//    FEAElementBase::GeometryEle::e_[2] = g.e(GeometryEle::Z);
//}

//FEAElementBase::GeometryEle::~GeometryEle()
//{

//}
