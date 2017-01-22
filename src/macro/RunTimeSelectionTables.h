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

/*
 * macros to create a runtimeselctiontable
 * for base class. It is then possible to
 * call constructor of derived class
 */

#ifndef RUNTIMESELECTIONTABLES_H
#define RUNTIMESELECTIONTABLES_H

#include <memory>
#include <unordered_map>
//#include <unique_ptr.h>
#include <QString>
#include <iostream>
#include <utility>
#include <functional>


#define declareRunTimeSelectionTable(unique_ptr, baseType, \
    argNames, argList, parList)\
    \
    typedef unique_ptr<baseType> (*argNames##ConstructorPtr)argList;\
    \
    typedef std::unordered_map<std::string,argNames##ConstructorPtr> \
    argNames##ConstructorTable;\
    \
    static argNames##ConstructorTable* argNames##ConstructorTablePtr_; \
    static void construct##argNames##ConstructorTables();\
    static void destroy##argNames##ConstructorTables();\
    \
    \
    /* derived class baseType##Type */\
    template<class baseType##Type>\
    class add##argNames##ConstructorToTable\
    {\
    public:\
    \
        static unique_ptr<baseType> New argList\
        {\
            return unique_ptr<baseType>(new baseType##Type parList);\
        }\
        \
        \
        /* constructor of this subclass */\
        add##argNames##ConstructorToTable \
        (\
            const std::string & lookup = baseType##Type::typeName\
        )\
        {\
           construct##argNames##ConstructorTables();\
           argNames##ConstructorTablePtr_->insert\
           (\
               std::make_pair(lookup, New)\
           );\
           if(0)\
           {\
                std::cerr << "Duplicate entry " << lookup\
                          << " in runtime selection table " << #baseType\
                          << std::endl;\
                /* error handle : better to use exception */\
                std::cout << std::cerr << std::endl;\
           }\
        }\
        \
        ~add##argNames##ConstructorToTable()\
        {\
            destroy##argNames##ConstructorTables();\
        }\
    };\
\

    /* declare a run-time selection for derived classes
     * we need this as the 1D (2D, 3D) element has serves as
     * the 1st level base class. Each of them will has its
     * own New function to create its own element */

#define declareRunTimeNewSelectionTable(unique_ptr, baseType, \
    argNames, argList, parList)\
    \
    typedef unique_ptr<baseType> (*argNames##ConstructorPtr)argList;\
    \
    typedef std::unordered_map<std::string,argNames##ConstructorPtr> \
    argNames##ConstructorTable;\
    \
    static argNames##ConstructorTable* argNames##ConstructorTablePtr_; \
    static void construct##argNames##ConstructorTables();\
    static void destroy##argNames##ConstructorTables();\
    \
    \
    /* derived class baseType##Type */\
    template<class baseType##Type>\
    class add##argNames##ConstructorToTable\
    {\
    public:\
    \
        static unique_ptr<baseType> New##baseType argList\
        {\
            return unique_ptr<baseType>((baseType##Type::New parList).get());\
        }\
        \
        \
        /* constructor of this subclass */\
        add##argNames##ConstructorToTable \
        (\
            const std::string & lookup = baseType##Type::typeName\
        )\
        {\
           construct##argNames##ConstructorTables();\
           argNames##ConstructorTablePtr_->insert\
           (\
                std::make_pair(lookup, New##baseType)\
           );\
           if(\
                0\
             )\
           {\
                std::cerr << "Duplicate entry " << lookup\
                          << " in runtime selection table " << #baseType\
                          << std::endl;\
                /* error handle : better to use exception */\
                std::cout << std::cerr << std::endl;\
           }\
        }\
        \
        ~add##argNames##ConstructorToTable()\
        {\
            destroy##argNames##ConstructorTables();\
        }\
    };\

/* constructor aid */
#define defineRunTimeSelectionTableConstructor(baseType, argNames) \
    \
    void baseType::construct##argNames##ConstructorTables()\
    {\
        static bool constructed = false;\
        if(!constructed) {\
            constructed = true;\
            baseType::argNames##ConstructorTablePtr_ \
                = new baseType::argNames##ConstructorTable;\
        }\
    }\

/* destructor aid */
#define defineRunTimeSelectionTableDestructor(baseType, argNames) \
    \
    void baseType::destroy##argNames##ConstructorTables()\
    {\
        if(baseType::argNames##ConstructorTablePtr_) {\
            delete baseType::argNames##ConstructorTablePtr_; \
            baseType::argNames##ConstructorTablePtr_ = NULL;\
        }\
    }\

/* create pointer to hash-table of functions */
#define defineRunTimeSelectionTablePtr(baseType, argNames)\
    \
    baseType::argNames##ConstructorTable *\
        baseType::argNames##ConstructorTablePtr_ = NULL \

/* interface to the classes */

#define defineRunTimeSelectionTable(baseType, argNames) \
    \
    defineRunTimeSelectionTablePtr(baseType, argNames);\
    defineRunTimeSelectionTableConstructor(baseType, argNames);\
    defineRunTimeSelectionTableDestructor(baseType, argNames)


#endif // RUNTIMESELECTIONTABLES_H
