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

#include "MaterialEle.h"

namespace NumToolKit {

namespace Fea {

const std::unordered_map<std::string, MaterialOne> \
MaterialEle::materialType \
= \
{   /* Name     , {rho,   E,   G,   v}*/ \
    {"Aluminum", {2.7e3, 70e9, 26e9, 0.35} },\
    {"IronFe", {7.874e3, 211e9, 82e9, 0.29} },\
    {"StainlessSteelGrade304",{7.85e3,203e9,81e9,0.275} }
};

MaterialEle::MaterialEle()
{

}

MaterialEle::MaterialEle(double rho, double E, double G)
{
    rho_ = rho;
    E_ = E;
    G_ = G;
}

MaterialEle::MaterialEle(MaterialEle &m)
{
    rho_ = m.rho();
    E_ = m.E();
    G_ = m.G();
    nu_ = m.nu();
}

MaterialEle::MaterialEle(std::string &name)
{
    std::unordered_map<std::string, MaterialOne>::const_iterator it;
    it = materialType.find("A");
    if(it != materialType.end()) {
        rho_ = (it->second).rho;
        E_ = (it->second).E;
        G_ = (it->second).G;
        nu_ = (it->second).nu;
    }
}

MaterialEle::~MaterialEle()
{

}

}

}
