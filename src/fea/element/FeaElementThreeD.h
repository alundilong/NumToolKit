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

#ifndef FEAELEMENTTHREED_H
#define FEAELEMENTTHREED_H

#include "FeaElementBase.h"

namespace NumToolKit {

namespace Fea {

class FEAElementThreeD : public FEAElementBase
{
public:
    static const std::string typeName;

    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
            std::unique_ptr,
            FEAElementThreeD,
            ElementName,
            (
                const std::string & dim,
                const std::string & name,
                const MaterialEle & m,
                const GeometryEle & g
             ),
            (
                dim,
                name,
                m,
                g
            )
    );

public:
    FEAElementThreeD();
    FEAElementThreeD
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle &m,
            const GeometryEle &g
    );

    // Selector
    static std::unique_ptr<FEAElementThreeD> &New
    (
            const std::string &  dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );
};

}

}

#endif // FEAELEMENTTHREED_H
