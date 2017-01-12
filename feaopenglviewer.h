#ifndef FEAOPENGLVIEWER_H
#define FEAOPENGLVIEWER_H

#include <QOpenGLWidget>

class FeaOpenGLViewer : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit FeaOpenGLViewer(QOpenGLWidget *parent = 0);

    bool & meshLoadedState() { return meshLoadedState_; }
    void setMesh(Mesh*mesh);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);


public slots:

    // solts for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

private:
    void draw();
    void drawMesh();

    inline const Mesh* mesh() const { return mesh_; }

    int xRot;
    int yRot;
    int zRot;
    bool meshLoadedState_;
    Mesh *mesh_;

    QPoint lastPos;

};

#endif // FEAOPENGLVIEWER_H
