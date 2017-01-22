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

#ifndef MATERIALELE_H
#define MATERIALELE_H

#include <string>
#include <unordered_map>
#include "MaterialOne.h"

namespace NumToolKit {

namespace Fea {

class MaterialEle
{

public:

//    static const int nMaterialType = 2;
    static const std::unordered_map<std::string, MaterialOne> \
    materialType;

    MaterialEle();
    MaterialEle(double rho, double E, double G);
    MaterialEle(MaterialEle &m);
    MaterialEle(std::string &name);
    ~MaterialEle();

    const double & rho() const { return rho_; }
    const double & E() const { return E_; }
    const double & G() const { return G_; }
    const double & nu() const { return nu_; }

private:
    double rho_;
    double E_;
    double G_;
    double nu_;

};

}

}

#endif // MATERIALELE_H
