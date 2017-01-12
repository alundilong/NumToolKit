#include "coordsystem.h"

coordSystem::coordSystem()
{
    // default system
    origin0_ = QVector3D(0, 0, 0);
    e0_ = QVector3D(1,0,0);
    e1_ = QVector3D(0,1,0);
    e2_ = QVector3D(0,0,1);
}

coordSystem::coordSystem(QVector3D origin0, QVector3D vx, QVector3D vy, QVector3D vz)
{
    origin0_ = origin0;
    e0_ = vx;
    e1_ = vy;
    e2_ = vz;
}
