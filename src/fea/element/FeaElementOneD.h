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

#ifndef FEAELEMENTONED_H
#define FEAELEMENTONED_H

#include "FeaElementBase.h"

namespace NumToolKit {

namespace Fea {


class FEAElementOneD : public FEAElementBase
{
public:
    static const std::string typeName;

    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
            std::unique_ptr,
            FEAElementOneD,
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
    FEAElementOneD();
    FEAElementOneD
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle &m,
            const GeometryEle &g
    );

    // Selector
    static std::unique_ptr<FEAElementOneD> New
    (
            const std::string &  dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );

protected:

    virtual void infoAboutThisElement();
    virtual void constructGeometry();
    virtual void constructBaseMatrix();
    virtual void transformToGlobal();
    virtual void numberSequence();

protected:
    double area_[2];
    double volume_;
    inline const double & volume() const { return volume_;}
    inline double area() const { return (area_[0]+area_[1])*0.5; }
};

}

}

#endif // FEAELEMENTONED_H
