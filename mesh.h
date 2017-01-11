#ifndef MESH_H
#define MESH_H

#include <QList>
#include <QString>
#include <QVector3D>
#include <QMap>

struct bound {
    double xlow;
    double xhig;
    double ylow;
    double yhig;
    double zlow;
    double zhig;
};

class Mesh
{
public:

    static const QString boundaryFileName;
    static const QString faceFileName;
    static const QString ownerFileName;
    static const QString neighbourFileName;
    static const QString pointFileName;

public:
    Mesh();
    Mesh(QString&);
    ~Mesh();

    const QList<int> faceNodes(int faceId) const; // return node
    const QList<int> cellNodes(int cellId) const; // return node
    const QList<int> cellFaces(int cellId) const; // return face

    // inline function
    inline int nCells() const { return nCells_; }
    inline int nFaces() const { return nFaces_; }
    inline int nNodes() const { return nNodes_; }
    inline bool meshState() const { return meshState_; }

    inline const QList<QList<int> > faces() const { return faces_; }
    inline const QList<int> owner() const { return owner_; }
    inline const QList<int> neighbour() const { return neighbour_; }
    inline const QList<QVector3D> points() const { return points_; }
    inline const QList<QVector3D> faceNormals() const { return faceNormals_; }
    inline const QList<QString> boundaryName() \
    const { return boundaryName_; }

    inline const QMap<QString, QList<int> > boundaryNameFaces() \
    const { return boundaryNameFaces_; }

    inline const QList< QList<int> > faceNode() const  { return faces_;}
    inline const QList< QList<int> > cellNode() const;
    inline const QList< QList<int> > cellFace() const { return cellFace_;}

    inline const bound box() const { return box_; }

private:

    QList<QString> boundaryName_; // name of BC
    QMap<QString, QList<int> > boundaryNameFaces_; //  a list of face id
    QList< QList<int> > faces_; // a list of list of face Id
    QList<int> owner_; // owner of face
    QList<int> neighbour_; // neighbor of face
    QList<QVector3D> points_; // list of point
    QList<QVector3D> faceNormals_; // normal of each face

    int nCells_; // total number of cells
    int nFaces_; // total number of faces
    int nNodes_; // total number of nodes

    bool meshState_;

    QList< QList<int> > faceNode_; // find node from face
    QList< QList<int> > cellNode_; // find node from cell
    QList< QList<int> > cellFace_; // find face from cell

    bool readPoints(QString &dir);
    bool readFaces(QString &dir);
    bool readBoundary(QString &dir);
    bool readOwner(QString &dir);
    bool readNeighbour(QString &dir);

    void createCellFaces();
    void createCellNodes();
    void createFaceNormals();

    bound box_;

};

#endif // MESH_H
