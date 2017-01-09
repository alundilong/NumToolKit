#ifndef MESH_H
#define MESH_H

#include <QList>

class Mesh
{
public:
    Mesh();
    ~Mesh();

private:
    QList<int> cells_; // a list of cell Id
    QList<int> faces_; // a list of face Id
    QList<int> nodes_; // a list of node Id
    QList< QList<int> > cellFace_; // read from stream
    QList< QList<int> > cellNode_; // read form stream

    const QList<int> cellNodes(int cellId) const; // return node
    const QList<int> cellFaces(int cellId) const; // return node

};

#endif // MESH_H
