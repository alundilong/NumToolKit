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

#ifndef MATHEXTENSIONIO_H
#define MATHEXTENSIONIO_H

std::ostream &operator<<(std::ostream& os, const mathExtension::Matrix &m)
{
    int nr = m.nrow();
    int nc = m.ncol();
    for(int i = 0; i < nr; i++ ) {
        for(int j = 0; j < nc; j++) {
            os << m[i][j] << ' ';
        }
        os << '\n';
    }
    return os;
}


#endif // MATHEXTENSIONIO_H
