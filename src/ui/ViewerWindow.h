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

#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QGLWidget>
#include "../fea/mesh/Mesh.h"
#include "../fea/mesh/STLMesh.h"

using namespace std;
using namespace NumToolKit::Fea;
// #include <QOpenGLWidget>

class ViewerWindow : public QGLWidget
// class ViewerWindow : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit ViewerWindow(QWidget *parent = 0);

    void setMeshLoadedState( bool state);
    void setSTLMeshLoadedState(bool state);
    void setFormat(QString format);
    void setMesh(Mesh*mesh);
    void setSTLMesh(STLMesh * stlmesh);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
//    void glDraw();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

signals:
    // signaling rotation from mouse movement
//    void xRotationChanged(int angle);
//    void yRotationChanged(int angle);
//    void zRotationChanged(int angle);

public slots:

    // solts for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

private:
    void draw();
    void drawMesh();
    void drawSTLMesh();

    inline const Mesh* mesh() const { return mesh_; }
    inline const STLMesh* stlmesh() const { return stlmesh_; }
    inline const double zoomVal() const { return zoomVal_; }
    inline const QString format() const { return format_; }

    inline const bool meshLoadedState() const { return meshLoadedState_; }
    inline const bool stlmeshLoadedState() const { return stlmeshLoadedState_; }

    int xRot;
    int yRot;
    int zRot;
    bool meshLoadedState_;
    bool stlmeshLoadedState_;
    QString format_;

    Mesh *mesh_;
    STLMesh *stlmesh_;

    QPoint lastPos;
    double zoomVal_;

};

#endif // VIEWERWINDOW_H
