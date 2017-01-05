#include "feaanalysispanel.h"
#include "ui_feaanalysispanel.h"

feaAnalysisPanel::feaAnalysisPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::feaAnalysisPanel)
{
    ui->setupUi(this);
    connect(ui->myGLwidget, SIGNAL(xRotationChanged(int)), ui->xRotSlider, SLOT(setValue(int)));
    connect(ui->myGLwidget, SIGNAL(yRotationChanged(int)), ui->yRotSlider, SLOT(setValue(int)));
    connect(ui->myGLwidget, SIGNAL(zRotationChanged(int)), ui->zRotSlider, SLOT(setValue(int)));
}

feaAnalysisPanel::feaAnalysisPanel(MainWindow *mw, QWidget *parent):
    QWidget(parent),
    ui(new Ui::feaAnalysisPanel),
    mw(mw)
{
    ui->setupUi(this);
    connect(ui->myGLwidget, SIGNAL(xRotationChanged(int)), ui->xRotSlider, SLOT(setValue(int)));
    connect(ui->myGLwidget, SIGNAL(yRotationChanged(int)), ui->yRotSlider, SLOT(setValue(int)));
    connect(ui->myGLwidget, SIGNAL(zRotationChanged(int)), ui->zRotSlider, SLOT(setValue(int)));
}

feaAnalysisPanel::~feaAnalysisPanel()
{
    delete ui;
}

void feaAnalysisPanel::on_xRotSlider_sliderMoved(int position)
{
    ui->myGLwidget->setXRotation(position);
}

void feaAnalysisPanel::on_yRotSlider_sliderMoved(int position)
{
    ui->myGLwidget->setYRotation(position);
}

void feaAnalysisPanel::on_zRotSlider_sliderMoved(int position)
{
    ui->myGLwidget->setZRotation(position);
}
