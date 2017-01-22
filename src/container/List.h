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
#include <iostream>


// need shallowcopy
// need deep copy

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
    // allow to assign new value
    inline const T* data() const { return data_; }
    inline T* data() { return data_; }
    T & operator[](const int & index) { return data_[index]; }
    // allow to get value
    const T operator[](const int & index) const { return data_[index]; }

    //
    void operator=(List<T> & list);

    friend std::ostream &operator<<(std::ostream& os, const List<T>&v){
        int nr = v.size();
        for(int i = 0; i < nr; i++ ) {
            os << v[i] << ' ';
        }
        return os;
    }

    inline const int size() const { return size_; }

private:
    T *data_;
    int size_;
//    void zeroize();
};

}// end of NumToolKit


#endif // LIST_H
