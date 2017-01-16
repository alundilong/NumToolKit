/*
 * macros to create a runtimeselctiontable
 * for base class. It is then possible to
 * call constructor of derived class
 */

#ifndef RUNTIMESELECTIONTABLES_H
#define RUNTIMESELECTIONTABLES_H

#include <unordered_map>
#include <auto_ptr.h>
#include <QString>
#include <iostream>

#define declareRunTimeSelectionTable(auto_ptr, baseType, \
    argNames, argList, parList)\
    \
    typedef auto_ptr<baseType> (*argNames##ConstructorPtr)argList;\
    \
    typedef unordered_map<QString,argNames##ConstructorPtr> \
    argNames##ConstructorTable;\
    \
    static argName##ConstructorTable* argName##ConstructorPtr_; \
    static void construct##argNames##ConstructorTables();\
    static void destroy##argNames##ConstructorTables();\
    \
    \
    /* derived class baseType##Type */\
    template<class baseType##Type>\
    class add##argName##ConstructorToTable\
    {\
    public:\
    \
        static auto_ptr<baseType> New argList\
        {\
            return auto_ptr<baseType>(new baseType##Type parList);\
        }\
        \
        \
        /* constructor of this subclass */\
        add##argName##ConstructorToTable \
        (\
            const QString & lookup = baseType##Type::typeName\
        )\
        {\
           construct##argNames##ConstructorTables();\
           if(!argName##ConstructorTable* argName##ConstructorPtr_->insert(lookup, New))\
           {\
                std::cerr << "Duplicate entry " << lookup\
                          << " in runtime selection table " << #baseType\
                          << std::endl;\
                /* error handle : better to use exception */\
                std::cout << std::cerr << std::endl;\
           }\
        }\
        \
        ~add##argName##ConstructorToTable()\
        {\
            destroy##argNames##ConstructorTables();\
        }\
    };\
\

    /* declare a run-time selection for derived classes
     * we need this as the 1D (2D, 3D) element has serves as
     * the 1st level base class. Each of them will has its
     * own New function to create its own element */
#define declareRunTimeNewSelectionTable(auto_ptr, baseType, \
    argNames, argList, parList)\
    \
    typedef auto_ptr<baseType> (*argNames##ConstructorPtr)argList;\
    \
    typedef unordered_map<QString,argNames##ConstructorPtr> \
    argNames##ConstructorTable;\
    \
    static argName##ConstructorTable* argName##ConstructorPtr_; \
    static void construct##argNames##ConstructorTables();\
    static void destroy##argNames##ConstructorTables();\
    \
    \
    /* derived class baseType##Type */\
    template<class baseType##Type>\
    class add##argName##ConstructorToTable\
    {\
    public:\
    \
        static auto_ptr<baseType> New##baseType argList\
        {\
            return auto_ptr<baseType>(baseType##Type::New parList);\
        }\
        \
        \
        /* constructor of this subclass */\
        add##argName##ConstructorToTable \
        (\
            const QString & lookup = baseType##Type::typeName\
        )\
        {\
           construct##argNames##ConstructorTables();\
           if(!argName##ConstructorTable* \
            argName##ConstructorPtr_->insert(lookup, New##baseType))\
           {\
                std::cerr << "Duplicate entry " << lookup\
                          << " in runtime selection table " << #baseType\
                          << std::endl;\
                /* error handle : better to use exception */\
                std::cout << std::cerr << std::endl;\
           }\
        }\
        \
        ~add##argName##ConstructorToTable()\
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
            baseType::argName##ConstructorPtr_ \
                = new baseType::argNames##ConstructorTable;\
        }\
    }\

/* destructor aid */
#define defineRunTimeSelectionTableDestructor(baseType, argNames) \
    \
    void baseType::destroy##argNames##ConstructorTables()\
    {\
        if(baseType::argName##ConstructorPtr_) {\
            delete baseType::argName##ConstructorPtr_; \
            baseType::argName##ConstructorPtr_ = NULL;\
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
