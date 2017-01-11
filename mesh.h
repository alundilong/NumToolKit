#ifndef MESH_H
#define MESH_H

#include <QList>
#include <QString>
#include <QVector3D>

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

private:

    QList<QString> boundaryName_; // name of BC
    QList<int> boundaryFaces_; //  faces
    QList<int> faces_; // a list of face Id
    double owners_; // owner of face
    double neighbours_; // neighbor of face
    QList<QVector3D> points_; // list of point

    QList<int> cells_; // a list of cell Id

    QList< QList<int> > faceNode_; // find node from face
    QList< QList<int> > cellNode_; // find node from cell
    QList< QList<int> > cellFace_; // find face from cell

    const QList<int> faceNodes(int faceId) const; // return node
    const QList<int> cellNodes(int cellId) const; // return node
    const QList<int> cellFaces(int cellId) const; // return face

    void readPoints(QString &dir);
    void readFaces(QString &dir);
    void readBoundary(QString &dir);
    void readOwner(QString &dir);
    void readNeighbour(QString &dir);

};

#endif // MESH_H
