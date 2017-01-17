#ifndef FEAELEMENTTWOD_H
#define FEAELEMENTTWOD_H

#include "feaelementbase.h"

class FEAElementTwoD : public FEAElementBase
{
public:
    static const std::string typeName;

    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
            std::auto_ptr,
            FEAElementTwoD,
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

    FEAElementTwoD();
    FEAElementTwoD
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle &m,
            const GeometryEle &g
    );

    // Selector
    static std::auto_ptr<FEAElementTwoD> New
    (
            const std::string &  dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );
};

#endif // FEAELEMENTTWOD_H
