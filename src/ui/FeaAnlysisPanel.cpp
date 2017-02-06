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
#include "../fea/mesh/FeaTwoDMesh.h"
#include "../fea/mesh/FeaOneDMesh.h"

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
    if(mesh() == NULL) {
        QMessageBox::warning(NULL,"..", "Please Load Mesh first!");
        return;
    }
    mw_->retrieveLogFromFEAWindow();

    if(ui->comboBox1DElement->currentText() == "Bar") {
        log_ += "Solving 1-D Bar Problem...\n";
        //solveFEA();
        solve2DFEA();
    }
}

void feaAnalysisPanel::solveFEA()
{
// construct mesh with polyMesh
    const Mesh & polyMesh = (*mesh());
    const QList<QList<int> > &cellNode = polyMesh.cellNode();

    const int nElement = polyMesh.nCells();
    const int nUnknown = polyMesh.nNodes()*3;

    std::string nameMat = "Aluminum-2014T";
    QList<FEAElementLinearCubicalElement*> elements;
    for (int i = 0; i < nElement; i++) {
        MaterialEle *m = new MaterialEle(nameMat);
        const QList<int> & vertex = cellNode[i];
        GeometryEle *g = new GeometryEle(polyMesh, vertex);
        std::unique_ptr<FEAElementBase> parentEle = \
                FEAElementBase::New(\
                    "ThreeD",\
                    "LinearCubicalElementBeamThreeD",\
                    *m,\
                    *g);
        FEAElementLinearCubicalElement *lce =\
                static_cast<FEAElementLinearCubicalElement*>(parentEle.get());
        parentEle.release();
        elements.push_back(lce);
    }

    mathExtension::Matrix A(nUnknown , nUnknown);
    mathExtension::Vector b(nUnknown);
    mathExtension::Vector x(nUnknown);

    QList<FEAElementLinearCubicalElement*>::const_iterator it;
    qDebug() << "====== Form Linear Algebra Equations =====";
    for(it = elements.begin(); it != elements.end(); ++it) {
        const FEAElementLinearCubicalElement &ele = **it;
        const QList<int> &Rows= ele.nodeIds();
        // be aware of vertex id (our id starts from 0)
        A.assemblyMatrix(Rows, Rows, ele.baseStiff(),false, ele.nDOF); // index with no moveby
    }

    setBoundaryConditions(polyMesh,A,b);
    linearAlgebraSolver las(A, b, x);
    las.LUSolve_GSL();
//    las.GaussSeidelMethod();
//    std::cout << "ke" << std::endl;
//    std::cout << elements[0]->baseStiff() << std::endl;
//    std::cout << "me" << std::endl;
//    std::cout << elements[0]->baseMass() << std::endl;

//    Form *tmp = new Form(A,b,mw_);
//    tmp->show();

    // post-processing
    // output new positions
    // output ux, uy, uz
    writeData(polyMesh, x);

//    QList<int> vertex;
//    polyMesh.fetchBCUniqueVertex("Left", vertex);
//    QList<int>::const_iterator it2;
//    for (it2 = vertex.begin(); it2 != vertex.end(); ++it2) {
//        int id = *it2;
//        qDebug() << id << x[id*3] << x[id*3+1] << x[id*3+2];
//        int ID = id*3;
//        for (int i = 0; i < 3; i++) {
//            for (int j = 0; j < 3; j++) {
//                qDebug() << "A : " << A[ID+i][ID+j];
//            }
//        }
//        qDebug() << "b : " << b[ID] << b[ID+1] << b[ID+2];
//    }

}

void feaAnalysisPanel::solve2DFEA()
{
    // 2D classic plate
    // construct 2D mesh from 3D mesh
        const FEATwoDMesh polyMesh = FEATwoDMesh(QVector3D(0,0,1), *mesh());
        const QList<QList<int> > &elementNodes = polyMesh.elementNodes();

        const int nElement = polyMesh.nCells();
        std::string nameMat = "Aluminum-2014T";
        QList<ClassicPlateElement*> elements;
        for (int i = 0; i < nElement; i++) {
            MaterialEle *m = new MaterialEle(nameMat);
            const QList<int> & vertex = elementNodes[i];
            qDebug() << vertex ;
            GeometryEle *g = new GeometryEle(polyMesh, vertex);
            std::unique_ptr<FEAElementBase> parentEle = \
                    FEAElementBase::New(\
                        "TwoD",\
                        "ClassicPlate43",\
                        *m,\
                        *g);
            ClassicPlateElement *cpe =\
                    static_cast<ClassicPlateElement*>(parentEle.get());
            parentEle.release();
            elements.push_back(cpe);
        }

        const int nUnknown = polyMesh.nNodes()*ClassicPlateElement::nDOF;
        mathExtension::Matrix A(nUnknown , nUnknown);
        mathExtension::Vector b(nUnknown);
        mathExtension::Vector x(nUnknown);

        QList<ClassicPlateElement*>::const_iterator it;
        qDebug() << "====== Form Linear Algebra Equations =====";
        for(it = elements.begin(); it != elements.end(); ++it) {
            const ClassicPlateElement &ele = **it;
            const QList<int> &Rows= ele.nodeIds();
            // be aware of vertex id (our id starts from 0)
            A.assemblyMatrix(Rows, Rows, ele.baseStiff(),false, ele.nDOF); // index with no moveby
        }
        Form *tmp = new Form(A,b,mw_);
        tmp->show();

//        const QMap<QString, QList<int> > & boundaries = polyMesh.boundaryNameNodes();
}

void feaAnalysisPanel::setBoundaryConditions(const Mesh &polyMesh, Matrix &A, Vector &b)
{
    // set displacement on Left as fixed boundary
    QList<int> vertex;
    polyMesh.fetchBCUniqueVertex("Left", vertex);
//    qDebug() << "Left: " << vertex;
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
    double yhig = polyMesh.box().yhig;
    double ylow = polyMesh.box().ylow;
    double yl = yhig - ylow;
    polyMesh.fetchBCUniqueVertex("Right", vertex);
    for (vIt = vertex.begin(); vIt != vertex.end(); ++vIt) {
        int vertexId = *vIt;
        if(polyMesh.points()[vertexId].y()>(yhig - 0.20*yl)) {
            int rs = vertexId*3;
            b[rs+2] = -10000;
//            std::cout << "Top:" << vertexId << std::endl;
        } else if (polyMesh.points()[vertexId].y() < (ylow + 0.25*yl)) {
            int rs = vertexId*3;
            b[rs+2] = 10000;
//            std::cout << "Bot:" << vertexId << std::endl;
        }
//        b[rs+1] = -100;
    }
    vertex.clear();
}


void feaAnalysisPanel::writeData(const Mesh & polyMesh, const Vector &x)
{
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
        outstream << QString("FIELD attributes %1").arg(1);
        // ux
        outstream << QString("\nDISP %1 %2 float \n").arg(3).arg(x.nrow()/3);
        for (int i = 0; i < x.nrow(); i=i+3) {
            outstream << QString("%1 %2 %3\n").arg(x[i]).arg(x[i+1]).arg(x[i+2]);
        }

        // new xyz
        outstream << QString("\n new positions \n");
        for (int i = 0; i < x.nrow(); i=i+3) {
            const QVector3D & point = polyMesh.points()[(i/3)];
            double newx = x[i] + point.x();
            double newy = x[i+1] + point.y();
            double newz = x[i+2] + point.z();
            outstream << QString(" %1 %2 %3\n").arg(newx).arg(newy).arg(newz);
        }

        file.close();
    }

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
