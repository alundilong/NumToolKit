#include "mesh.h"
#include <QFile>
#include "openfoamqtextstream.h"
#include <QTextEdit>
#include <QMessageBox>
#include <QDebug>

const QString Mesh::boundaryFileName = "/boundary";
const QString Mesh::faceFileName = "/faces";
const QString Mesh::ownerFileName = "/owner";
const QString Mesh::neighbourFileName = "/neighbour";
const QString Mesh::pointFileName = "/points";

Mesh::Mesh()
{

}

Mesh::Mesh(QString & dir)
{
    readPoints(dir);
    readFaces(dir);
    readOwner(dir);
    readNeighbour(dir);
    readBoundary(dir);
}

Mesh::~Mesh()
{

}

const QList<int> Mesh::faceNodes(int faceId) const
{
    return faceNode_[faceId];
}

const QList<int> Mesh::cellNodes(int cellId) const
{
    return cellNode_[cellId];
}

const QList<int> Mesh::cellFaces(int cellId) const
{
    return cellFace_[cellId];
}

void Mesh::readPoints(QString &dir)
{
    QString fileName = dir + pointFileName;
    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(NULL,"..", fileName+" fail to open!");
        return;
    }

    OpenFOAMQTextStream in(&file);
    in.ignore(QString("//"),2);

    // size
    int size;
    in >> size;

    qDebug() << "size: " << size;

    in.ignore(QString("("),1);

    for (int i = 0; i < size; i++) {
        QString val;
        double point[3];
        for (int j = 0; j < 3; j++) {
            in >> val;
            val.remove(QRegExp("[()]"));
//            val.remove(")");
            point[j] = val.toDouble();

        }
//        qDebug() << point[0] << "\t" << point[1] << "\t" << point[2];
        points_.push_back(QVector3D(point[0],point[1],point[2]));
    }

    file.close();
}

void Mesh::readFaces(QString &dir)
{

}

void Mesh::readBoundary(QString &dir)
{

}

void Mesh::readOwner(QString &dir)
{

}

void Mesh::readNeighbour(QString &dir)
{

}
