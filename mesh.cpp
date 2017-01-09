#include "mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

const QList<int> Mesh::cellNodes(int cellId) const
{
    return cellNode_[cellId];
}

const QList<int> Mesh::cellFaces(int cellId) const
{
    return cellFace_[cellId];
}
