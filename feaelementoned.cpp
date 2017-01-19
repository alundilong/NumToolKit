#include "feaelementoned.h"

//const std::string FEAElementOneD::typeName = "OneD";

FEAElementOneD::FEAElementOneD()
    :
      FEAElementBase()
{
}

FEAElementOneD::FEAElementOneD\
(\
        const std::string &dimension, \
        const std::string &name, \
        const MaterialEle &m, \
        const GeometryEle &g\
)
    :
    FEAElementBase(dimension, name, m, g)
{

}

std::auto_ptr<FEAElementOneD> FEAElementOneD::New
(
        const std::string &dimension,
        const std::string &name,
        const MaterialEle &m,
        const GeometryEle &g
)
{
    typename ElementNameConstructorTable::iterator cstrIter =
    ElementNameConstructorTablePtr_->find(name);

    return std::auto_ptr<FEAElementOneD>
    (
        ((cstrIter->second))(dimension, name, m, g)
    );
}


//FEAElementOneD::addSpaceDimensionConstructorToTable<FEAElementOneD>
//addFEAElementOneDSpaceDimensionConstructorToFEAElementBaseTable_;

makeElement(SpaceDimension, FEAElementOneD, FEAElementOneD, OneD)
defineRunTimeSelectionTable(FEAElementOneD, ElementName);
