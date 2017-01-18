#include "coordsystem.h"

coordSystem::coordSystem()
    :
      origin0_(QVector3D(0, 0, 0)),
      e0_(QVector3D(1,0,0)),
      e1_(QVector3D(0,1,0)),
      e2_(QVector3D(0,0,1))
{

}

coordSystem::coordSystem\
(\
        QVector3D origin0, \
        QVector3D axisX, \
        QVector3D axisY, \
        QVector3D axisZ\
)
    :
      origin0_(origin0),
      e0_(axisX),
      e1_(axisY),
      e2_(axisZ)
{

}
