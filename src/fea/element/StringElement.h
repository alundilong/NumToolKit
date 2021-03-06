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

#ifndef STRINGELEMENT_H
#define STRINGELEMENT_H

#include "FeaElementOneD.h"


namespace NumToolKit {

namespace Fea {

class StringElement : public FEAElementOneD
{
public:
    static const std::string typeName;
    static const int  nNode;
    static const int nDOF;

    StringElement();
    StringElement
    (
            const std::string & dimension,
            const std::string & name,
            const MaterialEle &m,
            const GeometryEle &g
    );;
    ~StringElement();

protected:
    virtual void infoAboutThisElement();
    virtual void constructGeometry();
    virtual void constructBaseMatrix();
    virtual void transformToGlobal();
    virtual void numberSequence();
};

}

}
#endif // STRINGELEMENT_H
