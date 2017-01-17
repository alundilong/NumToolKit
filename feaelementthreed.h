#ifndef FEAELEMENTTHREED_H
#define FEAELEMENTTHREED_H

#include "feaelementbase.h"

class FEAElementThreeD : public FEAElementBase
{
public:
    static const std::string typeName;

    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
            std::auto_ptr,
            FEAElementThreeD,
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
    FEAElementThreeD();
    FEAElementThreeD
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle &m,
            const GeometryEle &g
    );

    // Selector
    static std::auto_ptr<FEAElementThreeD> New
    (
            const std::string &  dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );
};

#endif // FEAELEMENTTHREED_H
