#include "feaelementtwod.h"

//const std::string FEAElementTwoD::typeName = "TwoD";

FEAElementTwoD::FEAElementTwoD()
    :
      FEAElementBase()
{
}

FEAElementTwoD::FEAElementTwoD\
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


//FEAElementBase::addSpaceDimensionConstructorToTable<FEAElementTwoD>
//addFEAElementTwoDSpaceDimensionConstructorToFEAElementBaseTable_;

makeElement(SpaceDimension, FEAElementTwoD, FEAElementBase, TwoD)
defineRunTimeSelectionTable(FEAElementTwoD, ElementName);
