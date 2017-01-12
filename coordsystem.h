#ifndef COORDSYSTEM_H
#define COORDSYSTEM_H

#include <QVector3D>

class coordSystem
{
public:
    coordSystem();
    coordSystem(QVector3D, QVector3D, QVector3D, QVector3D);
private:

    // 3 base vectors
    QVector3D e0_;
    QVector3D e1_;
    QVector3D e2_;

    // origin0 of coordinate system
    QVector3D origin0_;
};

#endif // COORDSYSTEM_H
