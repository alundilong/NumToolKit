#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QGLWidget>
#include "mesh.h"
#include "stlmesh.h"

using namespace std;
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
