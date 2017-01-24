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
#include <QMessageBox>

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
    QList<QList<int> > vertexList;
    polyMesh.numberSequence(Mesh::Cubic, vertexList);

    const int nElement = polyMesh.nCells();
    const int nUnknown = polyMesh.nNodes()*3;

    std::string nameMat = "Aluminum-2014T";
    QList<FEAElementLinearCubicalElement*> elements;
    for (int i = 0; i < nElement; i++) {
        MaterialEle *m = new MaterialEle(nameMat);
        const QList<int> & vertex = vertexList[i];
        GeometryEle *g = new GeometryEle(polyMesh, vertex);
        std::unique_ptr<FEAElementBase> parentEle = \
                FEAElementBase::New(\
                    "ThreeD",\
                    "LinearCubicalElementBarThreeD",\
                    *m,\
                    *g);
        FEAElementLinearCubicalElement *lce =\
                static_cast<FEAElementLinearCubicalElement*>(parentEle.get());
        parentEle.release();
        elements.push_back(lce);
    }

    mathExtension::Matrix A(nUnknown , nUnknown);

    QList<FEAElementLinearCubicalElement*>::const_iterator it;

    qDebug() << "====== Form Linear Algebra Equations =====";

    for(it = elements.begin(); it != elements.end(); ++it) {
        const FEAElementLinearCubicalElement &ele = **it;
        const QList<int> &Rows= ele.geometry()->vertexIds();
        // be aware of vertex id (our id starts from 0)
        A.assemblyMatrix(Rows, Rows, ele.baseStiff(),false, ele.nDOF); // index with no moveby
    }

    mathExtension::Vector b(nUnknown);
    mathExtension::Vector x(nUnknown);

    // set displacement on Left as fixed boundary
    QList<int> vertex;
    polyMesh.fetchBCUniqueVertex("Left", vertex);
    qDebug() << "Left: " << vertex;
    QList<int>::const_iterator vIt;
    for (vIt = vertex.begin(); vIt != vertex.end(); ++vIt) {
        int vertexId = *vIt;
        int rs = vertexId*3;
        int cs = vertexId*3;
        for (int i = 0; i < 3; i++) {
            A.setZeroExceptRowCol(rs+i, cs+i);
            b[rs+i] = 0.0;
        }
    }
    vertex.clear();
    // set force
    polyMesh.fetchBCUniqueVertex("Right", vertex);
    qDebug() << "Right: " << vertex;
    for (vIt = vertex.begin(); vIt != vertex.end(); ++vIt) {
        int vertexId = *vIt;
        int rs = vertexId*3;
        b[rs+1] = 1000;
    }
    vertex.clear();

//    std::cout << "A matrix:" << std::endl;
//    for (int i = 0; i < A.nrow(); i++) {
//        for (int j = 0; j < A.ncol(); j++) {
//            std::cout << A[i][j] <<",";
//        }
//        std::cout << ";" << std::endl;
//    }

//    std::cout << "b: " << std::endl;
//    for (int i = 0; i < b.nrow(); i++) {
//        std::cout << b[i] << ",";
//    }

//    A = A*1e-9;
    linearAlgebraSolver las(A, b, x);
//    las.GaussElimination();
//    las.LUSolve(); // coefficient will be changed
//    las.GaussSeidelMethod();
    las.LUSolve_GSL();
//    std::cout << x << std::endl;

    // post-processing
    // output new position
    // output ux, uy, uz

    QString fileName = QFileDialog::getSaveFileName(this, \
                                                    tr("Save FEA data File"), \
                                                    "..", \
                                                    tr("Text Files (*.data)"));
    if (fileName != "") {
        QFile file(QFileInfo(fileName).absoluteFilePath());

        if (file.exists())
        {
            QMessageBox::StandardButton chosenButton
                    = QMessageBox::warning(this, \
                                           tr("File exists"), \
                                           tr("The file already exists. Do you want to overwrite it?"),
                                           QMessageBox::Ok | QMessageBox::Cancel,
                                           QMessageBox::Cancel);
            if (chosenButton != QMessageBox::Ok)
            {
                return; //Save was cancelled
            }
        }
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Failed to save file"));
            return; //Aborted
        }
        // ok to write data into vtk format
        QTextStream outstream(&file);
        outstream << QString("POINT_DATA %1 \n").arg(x.nrow()/3);
        outstream << QString("FIELD attributes %1").arg(3);
        // ux
        outstream << QString("\nux %1 %2 float \n").arg(1).arg(x.nrow()/3);
        for (int i = 0; i < x.nrow(); i=i+3) {
            outstream << QString("%1 ").arg(x[i]);
            if(i%9 == 0) outstream << "\n";
        }

        // uy
        outstream << QString("\nuy %1 %2 float \n").arg(1).arg(x.nrow()/3);
        for (int i = 1; i < x.nrow(); i=i+3) {
            outstream << QString("%1 ").arg(x[i]);
            if((i-1)%9 == 0) outstream << "\n";
        }

        // uz
        outstream << QString("\nuz %1 %2 float \n").arg(1).arg(x.nrow()/3);
        for (int i = 2; i < x.nrow(); i=i+3) {
            outstream << QString("%1 ").arg(x[i]);
            if((i - 2)%9 == 0) outstream << "\n";
        }

        // new xyz
        outstream << QString("\n new positions \n");
        for (int i = 0; i < x.nrow(); i=i+3) {
            const QVector3D & point = polyMesh.points()[(i/3)];
//            qDebug() << (i/3) << point;
            double newx = x[i]+point.x();
            double newy = x[i+1]+point.y();
            double newz = x[i+2]+point.z();
            outstream << QString(" %1 %2 %3 ").arg(newx).arg(newy).arg(newz);
            if(i%9 == 0) outstream << "\n";
        }

        file.close();
    }

//    display stiffness matrix
//    Form *tmp = new Form(A, b, mw_);
//    tmp->show();

//    display mass matrix
//    const mathExtension::Matrix &Att = elements[0]->baseMass();
//    Form *tmp2 = new Form(Att, b, mw_);
//    tmp2->show();

//    log_ += las.mylog();
//    log_ += "\n Results of this Bar Problem is :\n";
//    for (int i = 0; i < x.nrow(); i++) {
//        log_ += QString("Element %1 : Disp = %2 m\n").arg(i).arg(QString::number(x[i]));
//    }
//    mw_->retrieveLogFromFEAWindow();

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
