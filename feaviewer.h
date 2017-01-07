#ifndef FEAVIEWER_H
#define FEAVIEWER_H

#include <QGLWidget>
// #include <QOpenGLWidget>

class FeaViewer : public QGLWidget
// class FeaViewer : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit FeaViewer(QWidget *parent = 0);

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

    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;

};

#endif // FEAVIEWER_H
