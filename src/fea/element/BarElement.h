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

#ifndef BARELEMENT_H
#define BARELEMENT_H

#include "FeaElementOneD.h"

namespace NumToolKit {

namespace Fea {

class BarElement : public FEAElementOneD
{
public:
    static const std::string typeName;
    // element numbering sequence
    enum location {LEFT, RIGHT};
    static const int  nNode;
    static const int nDOF;
public:
    BarElement();
    BarElement
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );
    ~BarElement();

    // access
    const QList<int>& nodeIds() const {return pointIds_;}
protected:
    virtual void infoAboutThisElement();
    virtual void constructGeometry();
    virtual void constructBaseMatrix();
    virtual void transformToGlobal();
    virtual void numberSequence();


//    double dof_[24];

};

}

}


#endif // BARELEMENT_H
