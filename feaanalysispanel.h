#ifndef FEAANALYSISPANEL_H
#define FEAANALYSISPANEL_H

#include <QWidget>
#include "mesh.h"

namespace Ui {
class feaAnalysisPanel;
}

class MainWindow;

class feaAnalysisPanel : public QWidget
{
    Q_OBJECT

public:
    explicit feaAnalysisPanel(QWidget *parent = 0);
    explicit feaAnalysisPanel(MainWindow *mw, QWidget *parent = 0);
    ~feaAnalysisPanel();
    const QString & getLog() {
        return log_;
    }

signals:
    void meshLoaded();

private slots:
    void on_xRotSlider_sliderMoved(int position);

    void on_yRotSlider_sliderMoved(int position);

    void on_zRotSlider_sliderMoved(int position);

    void on_radio2DElement_toggled(bool checked);

    void on_radio1DElement_toggled(bool checked);

    void on_radio3DElement_toggled(bool checked);

    void on_buttonRun_clicked();

    void on_loadMesh_clicked();

private:
    Ui::feaAnalysisPanel *ui;
    MainWindow *mw;
    QString log_;

    Mesh *mesh_;

    inline const Mesh* mesh() { return mesh_; }

    void solve1DBar();
};

#endif // FEAANALYSISPANEL_H
