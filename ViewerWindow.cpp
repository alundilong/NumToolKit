/* -*- c++ -*- ----------------------------------------------------------
    NumToolKit - Numerical Simulation ToolKit
    Yijin Mao, ymao.mu@gmail.com

    This file is part of NumToolKit.

    NumToolKit is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NumToolKit is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NumToolKit.  If not, see <http://www.gnu.org/licenses/>.

    This software is distributed under
    the GNU General Public License.

    See the README file in the top-level NumToolKit directory.
------------------------------------------------------------------------- */

#include "ViewerWindow.h"
#include <QtOpenGL>
#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <sstream>

ViewerWindow::ViewerWindow(QWidget *parent) :
//    QGLWidget(parent)
//  QOpenGLWidget(parent)
  QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    setAutoBufferSwap(true);
    mesh_ = NULL;
    meshLoadedState_ = false; // default is false
    zoomVal_ = 0.0;

//    // prevent the widget from being cleared when beginning the QPainter
//    setAutoFillBackground(false);

}

void ViewerWindow::setMeshLoadedState(bool state)
{
    meshLoadedState_ = state;
}

void ViewerWindow::setSTLMeshLoadedState(bool state)
{
    stlmeshLoadedState_ = state;
}

void ViewerWindow::setFormat(QString format)
{
    format_ = format;
}

void ViewerWindow::setMesh(Mesh *mesh)
{
    mesh_ = mesh;
}

void ViewerWindow::setSTLMesh(STLMesh *stlmesh)
{
    stlmesh_ = stlmesh;
}


void ViewerWindow::initializeGL()
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

void ViewerWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot/16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot/16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot/16.0, 0.0, 0.0, 1.0);

    if(format() == QString("openfoam")) {
        // if mesh loaded draw mesh
        if(meshLoadedState()) {
            //        qDebug() <<"draw Monkey...";
            drawMesh();
        }
    } else if(format() == QString("stl")) {
        // if mesh loaded draw mesh
        if(stlmeshLoadedState()) {
            //        qDebug() <<"draw Monkey...";
            drawSTLMesh();
        }
    } else {
//        qDebug() <<"draw Default...";
        draw();
    }
}

void ViewerWindow::resizeGL(int width, int height)
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

//void ViewerWindow::glDraw()
//{
//    QGLWidget::glDraw();

////    if(hasFocus())
////    {
//        QPainter painter(this);

//        QPen pen(Qt::yellow, 4);
//        painter.setPen(pen);

//        painter.drawRect(0, 0, width(), height());
////    }

//    swapBuffers();

//}

QSize ViewerWindow::minimumSizeHint() const
{
    return QSize(50,50);
}

QSize ViewerWindow::sizeHint() const
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

void ViewerWindow::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void ViewerWindow::mouseMoveEvent(QMouseEvent *event)
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

void ViewerWindow::wheelEvent(QWheelEvent *event)
{
    double delta = double(event->delta());
    zoomVal_ += (delta/1200.0);
    updateGL();
}

void ViewerWindow::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != xRot) {
        xRot = angle;
//        emit xRotationChanged(angle);
        updateGL();
    }
}

void ViewerWindow::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != yRot) {
        yRot = angle;
//        emit yRotationChanged(angle);
        updateGL();
    }
}

void ViewerWindow::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != zRot) {
        zRot = angle;
//        emit zRotationChanged(angle);
        updateGL();
    }
}

void ViewerWindow::draw()
{
    glDisable(GL_LIGHTING);
    qglColor(Qt::red);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
// https://www.opengl.org/discussion_boards/showthread.php/132858-Draw-a-triangle-and-his-borders-too

    glOrtho(-2-zoomVal(), +2+zoomVal(),\
            -2-zoomVal(), +2+zoomVal(), \
            1.0-zoomVal(), 15.0+zoomVal());
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

    glEnable(GL_LIGHTING);
}

void ViewerWindow::drawMesh()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = {0, 0, 0.1, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    qglColor(Qt::yellow);
    const Mesh* msh = mesh();
    double xlow = msh->box().xlow;
    double xhig = msh->box().xhig;
    double ylow = msh->box().ylow;
    double yhig = msh->box().yhig;
    double zlow = msh->box().zlow;
    double zhig = msh->box().zhig;
    glOrtho(xlow-zoomVal(), xhig+zoomVal(), \
            ylow-zoomVal(), yhig+zoomVal(), \
            max(zlow-zoomVal(),zlow), min(zhig+zoomVal(),zhig));

    int ndraw = 0;
    while(ndraw < 2) {

        if(ndraw == 1) {
            qglColor(Qt::blue);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if(ndraw == 0) {
            qglColor(Qt::yellow);
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        }
        ndraw++;

        for (int faceI = 0 ; faceI < msh->nFaces(); faceI++) {

            glBegin(GL_QUADS);

            double nx = msh->faceNormals()[faceI].x();
            double ny = msh->faceNormals()[faceI].y();
            double nz = msh->faceNormals()[faceI].z();

            glNormal3f(nx, ny, nz);
            double p1x = msh->points()[msh->faceNodes(faceI)[0]].x();
            double p1y = msh->points()[msh->faceNodes(faceI)[0]].y();
            double p1z = msh->points()[msh->faceNodes(faceI)[0]].z();

//            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(p1x, p1y, p1z);
            double p2x = msh->points()[msh->faceNodes(faceI)[1]].x();
            double p2y = msh->points()[msh->faceNodes(faceI)[1]].y();
            double p2z = msh->points()[msh->faceNodes(faceI)[1]].z();

//            glColor3f(0.0f,1.0f,0.0f);
            glVertex3f(p2x, p2y, p2z);
            double p3x = msh->points()[msh->faceNodes(faceI)[2]].x();
            double p3y = msh->points()[msh->faceNodes(faceI)[2]].y();
            double p3z = msh->points()[msh->faceNodes(faceI)[2]].z();

//            glColor3f(0.0f,0.0f,1.0f);
            glVertex3f(p3x, p3y, p3z);
            double p4x = msh->points()[msh->faceNodes(faceI)[3]].x();
            double p4y = msh->points()[msh->faceNodes(faceI)[3]].y();
            double p4z = msh->points()[msh->faceNodes(faceI)[3]].z();

//            glColor3f(0.0f,0.5f,1.0f);
            glVertex3f(p4x, p4y, p4z);

            glEnd();
        }
    }
    glEnable(GL_LIGHTING);
}

void ViewerWindow::drawSTLMesh()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = {0, 0, 0.1, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    qglColor(Qt::white);
    const STLMesh* msh = stlmesh();
    double xlow = msh->box().xlow;
    double xhig = msh->box().xhig;
    double ylow = msh->box().ylow;
    double yhig = msh->box().yhig;
    double zlow = msh->box().zlow;
    double zhig = msh->box().zhig;
    glOrtho(xlow-zoomVal(), xhig+zoomVal(), \
            ylow-zoomVal(), yhig+zoomVal(), \
            max(zlow-zoomVal(),zlow), min(zhig+zoomVal(),zhig));

    int ndraw = 0;
    while(ndraw < 2) {

        if(ndraw == 1) {
            qglColor(Qt::blue);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if(ndraw == 0) {
            qglColor(Qt::yellow);
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        }
        ndraw++;

        for (int faceI = 0 ; faceI < msh->nFaces(); faceI++) {

            glBegin(GL_TRIANGLES);

//            qDebug() << "FaceI : " << faceI;
            double nx = msh->faceNormals()[faceI].x();
            double ny = msh->faceNormals()[faceI].y();
            double nz = msh->faceNormals()[faceI].z();

            glNormal3f(nx, ny, nz);
            for (int nodeI = 0; nodeI < 3; nodeI++) {
//                qDebug() << msh->faceNodes(faceI)[nodeI];
                double px = msh->points()[msh->faceNodes(faceI)[nodeI]].x();
                double py = msh->points()[msh->faceNodes(faceI)[nodeI]].y();
                double pz = msh->points()[msh->faceNodes(faceI)[nodeI]].z();
                glVertex3f(px, py, pz);
            }

            glEnd();
        }
    }
    glEnable(GL_LIGHTING);
}

//28800 	 86400 	 28800 	 28800
