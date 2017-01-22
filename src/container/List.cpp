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

#include "List.h"

namespace NumToolKit{

template<typename T>
List<T>::List()
    :
      data_(NULL), size_(1)
{
    data_ = new T[size_];
//    zeroize();
}

template<typename T>
List<T>::List(const size_t &size)
    :
      data_(NULL), size_(size)
{
    data_ = new T[size_];
//    zeroize();
}

template<typename T>
List<T>::~List()
{
    if(data_!= NULL) {
        delete [] data_;
    }
}


template<typename T>
void List<T>::operator=(List<T> &list)
{
    size_ = list.size();
//    data_ = list.data();
    for (int i = 0; i < size(); i++) {
        data_[i] = list.data()[i];
    }
}

//template<typename T>
//void List::zeroize()
//{
//    for (int i = 0; i < size(); i++) {
//        data_[i] = 0.0;
//    }
//}

template class List<int>;
template class List< List<int> >;

//template<typename T, size_t size>
//List::List()
//    :
//      data_(NULL)
//{
//    data_ = new T[size];
//}

//template<typename T, size_t size>
//List::~List()
//{
//    if(data_!= NULL) {
//        delete [] data_;
//    }
//}

} // end of namespace
