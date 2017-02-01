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

#ifndef FEAELEMENTBASE_H
#define FEAELEMENTBASE_H

#include <QString>
#include <QDebug>
#include "../material/MaterialEle.h"
#include "../geometry/GeometryEle.h"
#include "../math/MathExtension.h"
#include "../../macro/RunTimeSelectionTables.h"
#include "../../macro/RunTimeElementNodeTables.h"
#include "../../macro/RunTimeElementNodeDOFTables.h"
#include "elementLength.h"

/*
 * abstract class for finite element
 */
namespace NumToolKit {

namespace Fea {

//class MaterialEle;
//class GeometryEle;

class FEAElementBase
{
public:
    enum component {XX, YY, ZZ};

    // Declare run-time constructor selection table
    declareRunTimeNewSelectionTable
    (
            std::unique_ptr,
            FEAElementBase,
            SpaceDimension,
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
    )

    // Declare run-time ElementNode selection table
    declareRunTimeElementNodeTable
    (
            FEAElementBase,
            ElementName
    )

    // Declare run-time ElementNode selection table
    declareRunTimeElementNodeDOFTable
    (
            FEAElementBase,
            ElementName
    )

public:
    FEAElementBase();
    FEAElementBase
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );

    static std::unique_ptr<FEAElementBase>  New
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );

    virtual ~FEAElementBase();

    QString getLog() {return log_;}

    // access
    const int & dim() const { return dim_; }
    const int & nDOFEle() const { return nDOFEle_;}
    const int & nNodeEle() const { return nNodeEle_;}
    const QString & name() const { return name_;}
    const mathExtension::Matrix & baseMass() const { return baseMass_;}
    const mathExtension::Matrix & baseStiff() const { return baseStiff_;}

//    const MaterialEle & material() const { return *material_; }
//    const GeometryEle & geometry() const { return *geoInfo_; }
    const MaterialEle * material() const { return material_; }
    const GeometryEle * geometry() const { return geoInfo_; }



//    // material of this element
//    class MaterialEle {

//    public:
//        MaterialEle();
//        MaterialEle(double rho, double E, double G);
//        MaterialEle(MaterialEle &m);
//        ~MaterialEle();

//        double rho() { return rho_; }
//        double E() { return E_; }
//        double G() { return G_; }

//    private:
//        double rho_;
//        double E_;
//        double G_;

//    };

    // geometry info of this element, such as length
//    class GeometryEle {
//        enum component {X, Y, Z, XY, XZ, YZ};
//    public:
//        GeometryEle();
//        GeometryEle(double e[3], double I[6]);
//        GeometryEle(GeometryEle &g);
//        ~GeometryEle();
//        double e(component comp) { return e_[comp]; }
//        double I(component comp) { return I_[comp]; }

//    private:
//        double e_[3]; // length in x, y, z direction
//        double I_[6]; // momentum initia
//    };

private:

    const MaterialEle *material_; // material of this element
    const GeometryEle *geoInfo_; // geometry info of this element

protected:
    int dim_;  // dimension of element
    int nDOFEle_; // Degree of Freedom
    int nNodeEle_; // Num of Node for each element
    QString name_; // name of element
    QString log_; // report log

    elementLength exyz_;
    elementLength & exyz() { return exyz_; }

    double I[3];// moment of inertia

    mathExtension::Matrix baseMass_; // mass matrix
    mathExtension::Matrix baseStiff_; // stiffiness matrix

    // firstNode label of this type element
    // it serves as the origin of the local
    // coordinate system for this element
    // Knowing this is a very important
    int firstNode_;

    virtual void infoAboutThisElement() = 0;
    virtual void constructGeometry() = 0;
    virtual void constructBaseMatrix() = 0;
    virtual void transformToGlobal() = 0;
    virtual void numberSequence() = 0;

};


#define addElementToRunTimeSelectionTable(SS, elementType, baseElementType)\
    /*const std::string elementType::typeName = "A";*/ \
    \
    baseElementType::add##SS##ConstructorToTable<elementType>\
    \
    add##elementType##SS##ConstructorTo##baseElementType##Table_;

#define makeElement(SS, elementType, baseElementType, name) \
    const std::string elementType::typeName = #name;\
    \
    addElementToRunTimeSelectionTable(SS, elementType, baseElementType)

/* add element node information into top-level table */
#define addElementNodeToRunTimeElementNodeTable(SS, elementType, baseElementType)\
    \
    baseElementType::add##SS##ElementNodeToTable<elementType>\
    \
    add##elementType##SS##ElementNodeTo##baseElementType##Table_;

#define makeElementNode(SS, elementType, baseElementType) \
    addElementNodeToRunTimeElementNodeTable(SS, elementType, baseElementType)

/* add element node dof information into top-level table */
#define addElementNodeDOFToRunTimeElementNodeDOFTable(SS, elementType, baseElementType)\
    \
    baseElementType::add##SS##ElementNodeDOFToTable<elementType>\
    \
    add##elementType##SS##ElementNodeDOFTo##baseElementType##Table_;

#define makeElementNodeDOF(SS, elementType, baseElementType) \
    addElementNodeDOFToRunTimeElementNodeDOFTable(SS, elementType, baseElementType)
}

}
#endif // FEAELEMENTBASE_H
