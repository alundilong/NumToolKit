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

#ifndef RUNTIMEELEMENTNODETABLES_H
#define RUNTIMEELEMENTNODETABLES_H

#include <memory>
#include <unordered_map>
#include <QString>
#include <iostream>
#include <utility>
#include <functional>


#define declareRunTimeElementNodeTable(baseType, \
    argNames)\
    \
     /*typedef auto_ptr<baseType> (*argNames##ElementNodePtr)argList;*/ \
    /*it is a name vs node table*/ \
    typedef std::unordered_map<std::string,const int> \
    argNames##ElementNodeTable;\
    \
    static argNames##ElementNodeTable* argNames##ElementNodeTablePtr_; \
    static void construct##argNames##ElementNodeTables();\
    static void destroy##argNames##ElementNodeTables();\
    \
    \
    /* derived class baseType##Type */\
    template<class baseType##Type>\
    class add##argNames##ElementNodeToTable\
    {\
    public:\
        \
        /* ElementNode of this subclass */\
        add##argNames##ElementNodeToTable \
        (\
            const std::string & lookup = baseType##Type::typeName\
        )\
        {\
           construct##argNames##ElementNodeTables();\
           argNames##ElementNodeTablePtr_->insert\
           (\
               std::make_pair(lookup, baseType##Type::nNode)\
           );\
           if(0)\
           {\
                std::cerr << "Duplicate entry " << lookup\
                          << " in runtime ElementNode table " << #baseType\
                          << std::endl;\
                /* error handle : better to use exception */\
                std::cout << std::cerr << std::endl;\
           }\
        }\
        \
        ~add##argNames##ElementNodeToTable()\
        {\
            destroy##argNames##ElementNodeTables();\
        }\
    };\
\

/* ElementNode aid */
#define defineRunTimeElementNodeTableElementNode(baseType, argNames) \
    \
    void baseType::construct##argNames##ElementNodeTables()\
    {\
        static bool constructed = false;\
        if(!constructed) {\
            constructed = true;\
            baseType::argNames##ElementNodeTablePtr_ \
                = new baseType::argNames##ElementNodeTable;\
        }\
    }\

/* destructor aid */
#define defineRunTimeElementNodeTableDestructor(baseType, argNames) \
    \
    void baseType::destroy##argNames##ElementNodeTables()\
    {\
        if(baseType::argNames##ElementNodeTablePtr_) {\
            delete baseType::argNames##ElementNodeTablePtr_; \
            baseType::argNames##ElementNodeTablePtr_ = NULL;\
        }\
    }\

/* create pointer to hash-table of functions */
#define defineRunTimeElementNodeTablePtr(baseType, argNames)\
    \
    baseType::argNames##ElementNodeTable *\
        baseType::argNames##ElementNodeTablePtr_ = NULL \

/* interface to the classes */

#define defineRunTimeElementNodeTable(baseType, argNames) \
    \
    defineRunTimeElementNodeTablePtr(baseType, argNames);\
    defineRunTimeElementNodeTableElementNode(baseType, argNames);\
    defineRunTimeElementNodeTableDestructor(baseType, argNames)

#endif // RUNTIMEELEMENTNODETABLES_H
