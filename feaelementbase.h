#ifndef FEAELEMENTBASE_H
#define FEAELEMENTBASE_H

#include <QString>
#include <QDebug>
#include "materialele.h"
#include "geometryele.h"
#include "mathextension.h"
#include "runTimeSelectionTables.h"

/*
 * abstract class for finite element
 */

//class MaterialEle;
//class GeometryEle;

class FEAElementBase
{
public:
    enum component {XX, YY, ZZ};

    // Declare run-time constructor selection table
    declareRunTimeNewSelectionTable
    (
            std::auto_ptr,
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

public:
    FEAElementBase();
    FEAElementBase
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );

    static std::auto_ptr<FEAElementBase> New
    (
            const std::string & dim,
            const std::string & name,
            const MaterialEle & m,
            const GeometryEle & g
    );

    ~FEAElementBase();

    QString getLog() {return log_;}

    // access
    const int & dim() const { return dim_; }
    const int & nDOFEle() const { return nDOFEle_;}
    const int & nNodeEle() const { return nNodeEle_;}
    const QString & name() const { return name_;}
    const mathExtension::Matrix & baseMass() const { return baseMass_;}
    const mathExtension::Matrix & baseStiff() const { return baseStiff_;}

    const MaterialEle & material() const { return *material_; }
    const GeometryEle & geometry() const { return *geoInfo_; }
    const MaterialEle * material() { return material_; }
    const GeometryEle * geometry() { return geoInfo_; }

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

    double I[3];// moment of inertia

    mathExtension::Matrix baseMass_; // mass matrix
    mathExtension::Matrix baseStiff_; // stiffiness matrix

    // firstNode label of this type element
    // it serves as the origin of the local
    // coordinate system for this element
    // Knowing this is a very important
    int firstNode_;

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

#endif // FEAELEMENTBASE_H
