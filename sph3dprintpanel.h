#ifndef SPH3DPRINTPANEL_H
#define SPH3DPRINTPANEL_H

#include <QWidget>
#include "viewerwindow.h"

namespace Ui {
class SPH3DPrintPanel;
}

class MainWindow;

class SPH3DPrintPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SPH3DPrintPanel(QWidget *parent = 0);
    explicit SPH3DPrintPanel(MainWindow *mw, ViewerWindow *vwin, QWidget *parent = 0);
    ~SPH3DPrintPanel();

signals:
    void stlmeshLoaded();

private slots:
    void on_loadStlMeshBotton_clicked();

    void on_dumpButton_clicked();

    void on_SetPathToInFile_clicked();

    void on_setPathToExe_clicked();

    void on_printButton_clicked();

private:
    Ui::SPH3DPrintPanel *ui;
    MainWindow *mw_;
    ViewerWindow *vwin_;
    STLMesh *stlmesh_;
    QString path2ExE_;
    QList<QVector3D> sphParticles_;

    inline const STLMesh* stlmesh() { return stlmesh_; }
    inline const QString path2ExE() { return path2ExE_;}
    inline const QList<QVector3D> sphParticles() { return sphParticles_; }
};

#endif // SPH3DPRINTPANEL_H
