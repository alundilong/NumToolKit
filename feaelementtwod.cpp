#include "feaelementtwod.h"

const std::string FEAElementTwoD::typeName = "TwoD";

FEAElementTwoD::FEAElementTwoD()
{
}

FEAElementTwoD::FEAElementTwoD\
(\
        const std::string &dim, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g\
)
{

}

std::auto_ptr<FEAElementTwoD> FEAElementTwoD::New
(
        const std::string &dimension,
        const std::string &name,
        const MaterialEle &m,
        const GeometryEle &g
)
{
    typename ElementNameConstructorTable::iterator cstrIter =
    ElementNameConstructorTablePtr_->find(name);

    return std::auto_ptr<FEAElementTwoD>
    (
        ((cstrIter->second))(dimension, name, m, g)
    );
}


FEAElementBase::addSpaceDimensionConstructorToTable<FEAElementTwoD>
addFEAElementTwoDSpaceDimensionConstructorToFEAElementBaseTable_;

defineRunTimeSelectionTable(FEAElementTwoD, ElementName);
