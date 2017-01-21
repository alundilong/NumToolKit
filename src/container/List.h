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

#ifndef LIST_H
#define LIST_H

#include "cstddef"

namespace NumToolKit{

//template<typename T, size_t size>
//class List
//{
//public:
//    List();
//    ~List();

//    // access to the data
//    inline const T* data() const { return data_; }
//    const T & operator[](size_t & index) { return data_[index]; }

//private:
//    T *data_;
//};

template<typename T>
class List
{
public:
    List();
    List(const size_t & size);
    ~List();

    // access to the data
    inline const T* data() const { return data_; }
    const T & operator[](int & index) const { return data_[index]; }
    inline const int size() const { return size_; }

private:
    T *data_;
    int size_;
};

}// end of NumToolKit


#endif // LIST_H
