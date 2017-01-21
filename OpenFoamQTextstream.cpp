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

#include "OpenFoamQTextstream.h"
#include <QDebug>

OpenFOAMQTextStream::OpenFOAMQTextStream(QFile* qf) : QTextStream(qf)
{
}


void OpenFOAMQTextStream::ignore(QString target, int max)
{
    int c = 0;
    QString tmp;
    OpenFOAMQTextStream & in = *this;
    while(!in.atEnd()) {
        (*this) >> tmp;
        if(tmp.compare(target) == 0) {
            c++;
        }
        if (c == max) break;
    }
}
