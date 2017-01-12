#ifndef STLMESH_H
#define STLMESH_H

#include <QString>
#include <QList>
#include <QVector3D>
#include "bound.h"

/*
 * this would be merged with mesh class
*/


class STLMesh
{
public:
    enum state {BAD = 0, GOOD = 1};
public:
    STLMesh();
    STLMesh(QString &fileName);

    const QList<int> faceNodes(int faceId) const; // return node
    const QList<int> cellNodes(int cellId) const; // return node
    const QList<int> cellFaces(int cellId) const; // return face

    // inline function
    inline int nCells() const { return nCells_; }
    inline int nFaces() const { return nFaces_; }
    inline int nNodes() const { return nNodes_; }
    inline bool meshState() const { return meshState_; }

    inline const bound box() const { return box_; }

    inline const QList< QList<int> > faceNode() const  { return faceNode_;}
    inline const QList< QList<int> > cellFace() const { return cellFace_;}

    inline const QList<QVector3D> points() const { return points_; }
    inline const QList<QVector3D> faceNormals() const { return faceNormals_; }

private:
    int nCells_;
    int nFaces_;
    int nNodes_;

    bool meshState_;

    QList< QList<int> > faceNode_; // find node from face
    QList< QList<int> > cellFace_; // find face from cell

    QList<QVector3D> points_; // list of point
    QList<QVector3D> faceNormals_; // normal of each face

    bound box_;
};

#endif // STLMESH_H
