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

#ifndef RUNTIMEELEMENTNODEDOFTABLES_H
#define RUNTIMEELEMENTNODEDOFTABLES_H

#include <memory>
#include <unordered_map>
#include <QString>
#include <iostream>
#include <utility>
#include <functional>


#define declareRunTimeElementNodeDOFTable(baseType, \
    argNames)\
    \
     /*typedef auto_ptr<baseType> (*argNames##ElementNodeDOFPtr)argList;*/ \
    /*it is a name vs node table*/ \
    typedef std::unordered_map<std::string,const int> \
    argNames##ElementNodeDOFTable;\
    \
    static argNames##ElementNodeDOFTable* argNames##ElementNodeDOFTablePtr_; \
    static void construct##argNames##ElementNodeDOFTables();\
    static void destroy##argNames##ElementNodeDOFTables();\
    \
    \
    /* derived class baseType##Type */\
    template<class baseType##Type>\
    class add##argNames##ElementNodeDOFToTable\
    {\
    public:\
        \
        /* ElementNodeDOF of this subclass */\
        add##argNames##ElementNodeDOFToTable \
        (\
            const std::string & lookup = baseType##Type::typeName\
        )\
        {\
           construct##argNames##ElementNodeDOFTables();\
           argNames##ElementNodeDOFTablePtr_->insert\
           (\
               std::make_pair(lookup, baseType##Type::nDOF)\
           );\
           if(0)\
           {\
                std::cerr << "Duplicate entry " << lookup\
                          << " in runtime ElementNodeDOF table " << #baseType\
                          << std::endl;\
                /* error handle : better to use exception */\
                std::cout << std::cerr << std::endl;\
           }\
        }\
        \
        ~add##argNames##ElementNodeDOFToTable()\
        {\
            destroy##argNames##ElementNodeDOFTables();\
        }\
    };\
\

/* ElementNodeDOF aid */
#define defineRunTimeElementNodeDOFTableElementNodeDOF(baseType, argNames) \
    \
    void baseType::construct##argNames##ElementNodeDOFTables()\
    {\
        static bool constructed = false;\
        if(!constructed) {\
            constructed = true;\
            baseType::argNames##ElementNodeDOFTablePtr_ \
                = new baseType::argNames##ElementNodeDOFTable;\
        }\
    }\

/* destructor aid */
#define defineRunTimeElementNodeDOFTableDestructor(baseType, argNames) \
    \
    void baseType::destroy##argNames##ElementNodeDOFTables()\
    {\
        if(baseType::argNames##ElementNodeDOFTablePtr_) {\
            delete baseType::argNames##ElementNodeDOFTablePtr_; \
            baseType::argNames##ElementNodeDOFTablePtr_ = NULL;\
        }\
    }\

/* create pointer to hash-table of functions */
#define defineRunTimeElementNodeDOFTablePtr(baseType, argNames)\
    \
    baseType::argNames##ElementNodeDOFTable *\
        baseType::argNames##ElementNodeDOFTablePtr_ = NULL \

/* interface to the classes */

#define defineRunTimeElementNodeDOFTable(baseType, argNames) \
    \
    defineRunTimeElementNodeDOFTablePtr(baseType, argNames);\
    defineRunTimeElementNodeDOFTableElementNodeDOF(baseType, argNames);\
    defineRunTimeElementNodeDOFTableDestructor(baseType, argNames)

#endif // RUNTIMEELEMENTNODEDOFTABLES_H
