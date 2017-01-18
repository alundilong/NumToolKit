#ifndef COORDSYSTEM_H
#define COORDSYSTEM_H

#include <QVector3D>

class coordSystem
{
public:
    coordSystem();
    coordSystem(QVector3D, QVector3D, QVector3D, QVector3D);

    // accessible to each element's constructor
    inline const QVector3D  & e0() const { return e0_;}
    inline const QVector3D  & e1() const { return e1_;}
    inline const QVector3D  & e2() const { return e2_;}

private:

    // must known members
    // 3 base vectors
    const QVector3D e0_;
    const QVector3D e1_;
    const QVector3D e2_;

    // origin0 of coordinate system
    const QVector3D origin0_;
};

#endif // COORDSYSTEM_H
