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

#include "FeaAnalysisPanel.h"
#include "ui_feaanalysispanel.h"
#include "MainWindow.h"
#include "../fea/math/LinearAlgebraSolver.h"

#include "../fea/element/ElementStyle.h"
//#include "../fea/mesh/FeaMesh.h"

#include <QDebug>
#include "Form.h"
#include <QFileDialog>

using namespace NumToolKit::Fea;

feaAnalysisPanel::feaAnalysisPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::feaAnalysisPanel),
    mesh_(NULL)
{
    ui->setupUi(this);

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

    connect(this, SIGNAL(meshLoaded()), vwin_, SLOT(updateGL()));
}

feaAnalysisPanel::feaAnalysisPanel(MainWindow *mw, ViewerWindow *vwin, QWidget *parent):
    QWidget(parent),
    ui(new Ui::feaAnalysisPanel),
    mw_(mw),
    vwin_(vwin),
    mesh_(NULL)
{
    ui->setupUi(this);

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

    connect(this, SIGNAL(meshLoaded()), vwin_, SLOT(updateGL()));
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(destroyed(QObject*)), vwin_, SLOT(close()));

//    mw->retrieveLogFromFEAWindow();
}

feaAnalysisPanel::~feaAnalysisPanel()
{
    delete ui;
}


void feaAnalysisPanel::on_radio2DElement_toggled(bool checked)
{
    ui->comboBox1DElement->setEnabled(!checked);
    ui->comboBox2DElement->setEnabled(checked);
    ui->comboBox3DElement->setEnabled(!checked);
}

void feaAnalysisPanel::on_radio1DElement_toggled(bool checked)
{

    ui->comboBox1DElement->setEnabled(checked);
    ui->comboBox2DElement->setEnabled(!checked);
    ui->comboBox3DElement->setEnabled(!checked);
}

void feaAnalysisPanel::on_radio3DElement_toggled(bool checked)
{

    ui->comboBox1DElement->setEnabled(!checked);
    ui->comboBox2DElement->setEnabled(!checked);
    ui->comboBox3DElement->setEnabled(checked);
}

void feaAnalysisPanel::on_buttonRun_clicked()
{
    mw_->retrieveLogFromFEAWindow();

    if(ui->comboBox1DElement->currentText() == "Bar") {
        log_ += "Solving 1-D Bar Problem...\n";
        solveFEA();
    }
}

void feaAnalysisPanel::solveFEA()
{
// construct mesh with polyMesh
    const Mesh & polyMesh = (*mesh());
// pre-unkown the shape of each cell (cube)
// find element's numbering sequence
    const List< List<int> > vertexList \
            = polyMesh.numberSequence(Mesh::Cubic);

    const int nElement = polyMesh.nCells();
    const int nNodes = polyMesh.nNodes();

    std::string nameMat = "Aluminum";
    // meshing manually
    QList<FEAElementLinearCubicalElement> elements;
    for (int i = 0; i < nElement; i++) {
        MaterialEle *m = new MaterialEle(nameMat);
        const List<int> & vertex = vertexList[i];
        GeometryEle *g = new GeometryEle(polyMesh, vertex);
        auto_ptr<FEAElementBase> parentEle = \
                FEAElementBase::New(\
                    "ThreeD",\
                    "LinearCubicalElementBarThreeD",\
                    *m,\
                    *g);
        FEAElementLinearCubicalElement *lce =\
                dynamic_cast<FEAElementLinearCubicalElement*>(parentEle.get());
        elements.push_back(*lce);
    }

    mathExtension::Matrix A(nNodes, nNodes);

    QList<FEAElementLinearCubicalElement>::const_iterator it;
    for(it = elements.begin(); it != elements.end(); ++it) {
        const FEAElementLinearCubicalElement & ele = *it;
        const List<int> & Rows = ele.geometry()->vertexIds();
        A.setSubMatrix(Rows, Rows, ele.baseStiff());
    }

    mathExtension::Vector b(nNodes);
    mathExtension::Vector x(nNodes);

    linearAlgebraSolver las(A, b, x);
    las.GaussElimination(); // coefficient will be changed

//    Form *tmp = new Form(N_p, K_p, Q_p, mw);
//    tmp->show();

    log_ += las.mylog();
    log_ += "\n Results of this Bar Problem is :\n";
    for (int i = 0; i < x.nrow(); i++) {
        log_ += QString("Element %1 : Disp = %2 m\n").arg(i).arg(QString::number(x[i]));
    }
    mw_->retrieveLogFromFEAWindow();

}

void feaAnalysisPanel::on_loadMesh_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory\
            (this,\
             "mesh dir",\
             "~/home",\
             QFileDialog::ShowDirsOnly);
    mesh_ = new Mesh(dirName);
    qDebug() << "feaPannel : number of cell : " << mesh()->nCells();

    if(mesh()->meshState()) {
        vwin_->setFormat(QString("openfoam"));
        vwin_->setMeshLoadedState(true);
        vwin_->setMesh(mesh_);
        emit meshLoaded();
    }
}
