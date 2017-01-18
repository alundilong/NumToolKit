#ifndef FEAELEMENTONED_H
#define FEAELEMENTONED_H

#include "feaelementbase.h"

class FEAElementOneD : public FEAElementBase
{
public:
    static const std::string typeName;

    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
            std::auto_ptr,
            FEAElementOneD,
            ElementName,
            (
                const std::string & dim,
                const std::string & name,
                const MaterialEle & m,
                const GeometryEle & g
             ),
            (
                dim,
                name,
                m,
                g
            )
    );

public:
    FEAElementOneD();
    FEAElementOneD
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle &m,
            const GeometryEle &g
    );

    // Selector
    static std::auto_ptr<FEAElementOneD> New
    (
            const std::string &  dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );
};

#endif // FEAELEMENTONED_H
