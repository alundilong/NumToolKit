#include "feaelementthreed.h"

const std::string FEAElementThreeD::typeName = "ThreeD";

FEAElementThreeD::FEAElementThreeD()
{
}

FEAElementThreeD::FEAElementThreeD\
(\
        const std::string &dim, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g\
)
{

}

std::auto_ptr<FEAElementThreeD> FEAElementThreeD::New
(
        const std::string &dimension,
        const std::string &name,
        const MaterialEle &m,
        const GeometryEle &g
)
{
    typename ElementNameConstructorTable::iterator cstrIter =
    ElementNameConstructorTablePtr_->find(name);

    return std::auto_ptr<FEAElementThreeD>
    (
        ((cstrIter->second))(dimension, name, m, g)
    );
}


FEAElementBase::addSpaceDimensionConstructorToTable<FEAElementThreeD>
addFEAElementThreeDSpaceDimensionConstructorToFEAElementBaseTable_;

defineRunTimeSelectionTable(FEAElementThreeD, ElementName);
