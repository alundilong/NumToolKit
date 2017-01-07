#include "feaanalysispanel.h"
#include "ui_feaanalysispanel.h"
#include "mainwindow.h"
#include "linearalgebrasolver.h"

#include "elementstyle.h"
#include <QDebug>

feaAnalysisPanel::feaAnalysisPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::feaAnalysisPanel)
{
    ui->setupUi(this);
    connect(ui->myGLwidget, SIGNAL(xRotationChanged(int)), ui->xRotSlider, SLOT(setValue(int)));
    connect(ui->myGLwidget, SIGNAL(yRotationChanged(int)), ui->yRotSlider, SLOT(setValue(int)));
    connect(ui->myGLwidget, SIGNAL(zRotationChanged(int)), ui->zRotSlider, SLOT(setValue(int)));

    // default : 1D elment
    ui->radio1DElement->setChecked(true);
    ui->comboBox1DElement->setEnabled(true);
    ui->comboBox1DElement->addItem("Bar");
    ui->comboBox1DElement->addItem("Beam (Euler-Bernoulli)");
    ui->comboBox1DElement->addItem("String");
    ui->radio2DElement->setChecked(false);
    ui->comboBox2DElement->setEnabled(false);
    ui->comboBox2DElement->addItem("Plate (Classic)");
    ui->comboBox2DElement->addItem("Membrane");
    ui->radio3DElement->setChecked(false);
    ui->comboBox3DElement->setEnabled(false);
    ui->comboBox3DElement->addItem("8 node");
    ui->comboBox3DElement->addItem("20 node (8+12)");
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

    // default : 1D elment
    ui->radio1DElement->setChecked(true);
    ui->comboBox1DElement->setEnabled(true);
    ui->comboBox1DElement->addItem("Bar");
    ui->comboBox1DElement->addItem("Beam (Euler-Bernoulli)");
    ui->comboBox1DElement->addItem("String");
    ui->radio2DElement->setChecked(false);
    ui->comboBox2DElement->setEnabled(false);
    ui->comboBox2DElement->addItem("Plate (Classic)");
    ui->comboBox2DElement->addItem("Membrane");
    ui->radio3DElement->setChecked(false);
    ui->comboBox3DElement->setEnabled(false);
    ui->comboBox3DElement->addItem("8 node");
    ui->comboBox3DElement->addItem("20 node (8+12)");

    log_ = "Finite Element Analysis Panel Loaded\n";

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

void feaAnalysisPanel::on_radio2DElement_toggled(bool checked)
{
//    ui->radio1DElement->setChecked(!checked);
    ui->comboBox1DElement->setEnabled(!checked);
//    ui->radio2DElement->setChecked(checked);
    ui->comboBox2DElement->setEnabled(checked);
//    ui->radio3DElement->setChecked(!checked);
    ui->comboBox3DElement->setEnabled(!checked);
}

void feaAnalysisPanel::on_radio1DElement_toggled(bool checked)
{
//    ui->radio1DElement->setChecked(checked);
    ui->comboBox1DElement->setEnabled(checked);
//    ui->radio2DElement->setChecked(!checked);
    ui->comboBox2DElement->setEnabled(!checked);
//    ui->radio3DElement->setChecked(!checked);
    ui->comboBox3DElement->setEnabled(!checked);
}

void feaAnalysisPanel::on_radio3DElement_toggled(bool checked)
{
//    ui->radio1DElement->setChecked(!checked);
    ui->comboBox1DElement->setEnabled(!checked);
//    ui->radio2DElement->setChecked(!checked);
    ui->comboBox2DElement->setEnabled(!checked);
//    ui->radio3DElement->setChecked(checked);
    ui->comboBox3DElement->setEnabled(checked);
}

void feaAnalysisPanel::on_buttonRun_clicked()
{
    mw->retrieveLogFromFEAWindow();

    // Need a mesh class to store geometry data (3D)
    // Mesh reads points, stored by label

    // 1D problem
    const int nElement = 3;
    BarElement *barElements = new BarElement[nElement];

    // BarElement elements[nElement];
    // solve the pre-defined problem
    // check dimension of the problem
    // check element type
    // check boundary condition
    // construct stiffness matrix A and b
    // define linear algebra solver obj
    // solve problem and return results and display
}
