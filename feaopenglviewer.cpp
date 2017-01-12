#include "feaopenglviewer.h"
#include <QtOpenGL>
#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <sstream>

FeaOpenGLViewer::FeaOpenGLViewer(QOpenGLWidget( *parent) :
  QOpenGLWidget(parent)
//  QOpenGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    setAutoBufferSwap(true);
    mesh_ = NULL;
    meshLoadedState_ = false; // default is false
}

void FeaOpenGLViewer::setMesh(Mesh *mesh)
{
    mesh_ = mesh;
}


void FeaOpenGLViewer::initializeGL()
{
    qglClearColor(Qt::black);
//    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = {0, 0, 10, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void FeaOpenGLViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot/16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot/16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot/16.0, 0.0, 0.0, 1.0);

    // if mesh loaded draw mesh
    if(meshLoadedState()) {
//        qDebug() <<"draw Monkey...";
        drawMesh();
    } else {
//        qDebug() <<"draw Default...";
        draw();
    }
}

void FeaOpenGLViewer::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side)/2, (height-side)/2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

#ifndef QT_OPENGL_ES_1
    glOrtho(-2, +2, -2, +2, 1.0, 15.0); // glOrthf(...)
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

QSize FeaOpenGLViewer::minimumSizeHint() const
{
    return QSize(50,50);
}

QSize FeaOpenGLViewer::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void FeaOpenGLViewer::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void FeaOpenGLViewer::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
       setXRotation(xRot + 8 * dy);
       setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    }

    lastPos = event->pos();
}

void FeaOpenGLViewer::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void FeaOpenGLViewer::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void FeaOpenGLViewer::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void FeaOpenGLViewer::draw()
{
    qglColor(Qt::red);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glVertex3f(-1, -1, 0);
        glVertex3f(-1, 1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, -1, 0);

    glEnd();

    glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(0,0,1.2);
        glEnd();
}

void FeaOpenGLViewer::drawMesh()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = {0, 0, 0.1, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    qglColor(Qt::yellow);
    const Mesh* msh = mesh();
    double xlow = 2*msh->box().xlow;
    double xhig = 2*msh->box().xhig;
    double ylow = 2*msh->box().ylow;
    double yhig = 2*msh->box().yhig;
    glOrtho(xlow, xhig, ylow, yhig, -1.0, 1.0);

    for (int faceI = 0 ; faceI < msh->nFaces(); faceI++) {
        glBegin(GL_QUADS);

//        qDebug() << "faceI : " << faceI << "node 1: " << msh->faceNodes(faceI)[0]\
//                 << " node 2: " << msh->faceNodes(faceI)[1] \
//                 << " node 3: " << msh->faceNodes(faceI)[2] \
//                 << " node 4: " << msh->faceNodes(faceI)[3];
        double nx = msh->faceNormals()[faceI].x();
        double ny = msh->faceNormals()[faceI].y();
        double nz = msh->faceNormals()[faceI].z();
//        qDebug() << QString("normal %1 %2 %3").arg(nx).arg(ny).arg(nz);
        glNormal3f(nx, ny, nz);
        double p1x = msh->points()[msh->faceNodes(faceI)[0]].x()*20;
        double p1y = msh->points()[msh->faceNodes(faceI)[0]].y()*20;
        double p1z = msh->points()[msh->faceNodes(faceI)[0]].z()*20;
//        qDebug() << QString("normal %1 %2 %3").arg(p1x).arg(p1y).arg(p1z);
        glVertex3f(p1x, p1y, p1z);
        double p2x = msh->points()[msh->faceNodes(faceI)[1]].x()*20;
        double p2y = msh->points()[msh->faceNodes(faceI)[1]].y()*20;
        double p2z = msh->points()[msh->faceNodes(faceI)[1]].z()*20;
//        qDebug() << QString("normal %1 %2 %3").arg(p2x).arg(p2y).arg(p2z);
        glVertex3f(p2x, p2y, p2z);
        double p3x = msh->points()[msh->faceNodes(faceI)[2]].x()*20;
        double p3y = msh->points()[msh->faceNodes(faceI)[2]].y()*20;
        double p3z = msh->points()[msh->faceNodes(faceI)[2]].z()*20;
//        qDebug() << QString("normal %1 %2 %3").arg(p3x).arg(p3y).arg(p3z);
        glVertex3f(p3x, p3y, p3z);
        double p4x = msh->points()[msh->faceNodes(faceI)[3]].x()*20;
        double p4y = msh->points()[msh->faceNodes(faceI)[3]].y()*20;
        double p4z = msh->points()[msh->faceNodes(faceI)[3]].z()*20;
//        qDebug() << QString("normal %1 %2 %3").arg(p4x).arg(p4y).arg(p4z);
        glVertex3f(p4x, p4y, p4z);

//        glNormal3f(0, 0, -1);
//        glVertex3f(-1, -1, 0);
//        glVertex3f(-1, 1, 0);
//        glVertex3f(1, 1, 0);
//        glVertex3f(1, -1, 0);

        glEnd();
    }
    glEnable(GL_LIGHTING);
}
