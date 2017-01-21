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
#include "LinearAlgebraSolver.h"

#include "ElementStyle.h"

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
        solve1DBar();
    }
}

void feaAnalysisPanel::solve1DBar()
{
//    |-----|-----|-----|
//    |-----|-----|-----|

    // Need a mesh class to store geometry data (3D)
    // Mesh reads points, stored by label

    // 1D problem
    const int nElement = 300;
    const int nNodes = nElement + 1;

    // geometry info
    const double length = 1.0; // m

    // material info
    const double rho = 7.9e3; // kg/m^3
    const double E = 1e9; // Pa // Young's Modulus
    const double G = 1e10; // Pa

    QList<BarElement> elements;
    for (int i = 0; i < nElement; i++) {
        double e[3] = {length/nElement, 0.1, 0.1};
        MaterialEle *m = new MaterialEle(rho,E,G);
        GeometryEle *g = new GeometryEle(e);
        BarElement *barEle = new BarElement("OneD","Bar",*m,*g);
        elements.push_back(*barEle);
    }

    // assembly the matrix
    const int N = nNodes; // total unknown before BC applied
    double **M = new double*[N]; // full mass matrix
    double **K = new double*[N]; // full stiff matrix
    double *Q = new double[N];
    double *X = new double[N]; // unknown

    for (int i = 0; i < N; i++) {
        M[i] = new double[N];
        K[i] = new double[N];
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            M[i][j] = 0.0;
            K[i][j] = 0.0;
        }
    }

    const int nNodeEle = BarElement::nNode;

    for (int k = 0; k < nElement; k++) {
        const BarElement & barEle = elements[k];
        int eleId = k; // to be general, a list storing element Id should be defined (MeshData)
        for (int i = 0; i < nNodeEle; i++) {
            int nodeI_Id = i + k; // to be general, a list of nodeId for each element should be defined (MeshData)
            for (int j = 0; j < nNodeEle; j++) {
                int nodeJ_Id = j + k;
                M[nodeI_Id][nodeJ_Id] += barEle.baseMass()[i][j];
                K[nodeI_Id][nodeJ_Id] += barEle.baseStiff()[i][j];

            }
        }
    }

    // apply boundary condition
    // left : u = 0;

    int nGBC = 1;
    int eleIDGBC = 0; // element ID with Geometry Boundary Condition
    int A2RemoveIndex = 0; // mesh.eleNodes()[BarElement::LEFT];// remove this element in A

    // right : F = 100 N;
    for (int i = 0; i < N; i++) {
        Q[i] = 0;
        X[i] = 0;
    }

    const int nDOFEle = BarElement::nDOF;
    int eleIDEOM = nElement-1; // element ID with Equation of Motion
    Q[eleIDEOM*nDOFEle + BarElement::RIGHT] = 100;

    // reconstruct M, K, and Q

    int N_p = N - nGBC;
    double **M_p = new double*[N_p];
    double **K_p = new double*[N_p];
    double *Q_p = new double[N_p];
    double *X_p = new double[N_p];

    for (int i = 0; i < N_p; i++) {
        M_p[i] = new double[N_p];
        K_p[i] = new double[N_p];
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i < A2RemoveIndex && j < A2RemoveIndex) {
                M_p[i][j] = M[i][j];
                K_p[i][j] = K[i][j];
            } else if (i < A2RemoveIndex && j > A2RemoveIndex) {
                M_p[i-1][j] = M[i][j];
                K_p[i-1][j] = K[i][j];
            } else if (i > A2RemoveIndex && j > A2RemoveIndex) {
                M_p[i-1][j-1] = M[i][j];
                K_p[i-1][j-1] = K[i][j];
            } else if (i > A2RemoveIndex && j < A2RemoveIndex) {
                M_p[i][j-1] = M[i][j];
                K_p[i][j-1] = K[i][j];
            }
        }

    }

    for (int i = 0; i < N; i++) {

        if (i < A2RemoveIndex) {Q_p[i] = Q[i]; X_p[i] = 0.0;}
        else if (i > A2RemoveIndex) {Q_p[i-1] = Q[i]; X_p[i-1] = 0.0;}

    }

    linearAlgebraSolver las(N_p, K_p, Q_p, X_p);
    las.GaussElimination(); // coefficient will be changed

//    Form *tmp = new Form(N_p, K_p, Q_p, mw);
//    tmp->show();

    log_ += las.mylog();
    log_ += "\n Results of this Bar Problem is :\n";
    for (int i = 0; i < N_p; i++) {
        log_ += QString("Element %1 : Disp = %2 m\n").arg(i).arg(QString::number(X_p[i]));
    }
    mw_->retrieveLogFromFEAWindow();

    // BarElement elements[nElement];
    // solve the pre-defined problem
    // check dimension of the problem
    // check element type
    // check boundary condition
    // construct stiffness matrix A and b
    // define linear algebra solver obj
    // solve problem and return results and display

    for (int i = 0; i < N; i++) {
        delete M[i];
        delete K[i];
    }
    delete [] M;
    delete [] K;
    delete [] Q;
    delete [] X;

    for (int i = 0; i < N_p; i++) {
        delete M_p[i];
        delete K_p[i];
    }
    delete [] M_p;
    delete [] K_p;
    delete [] Q_p;
    delete [] X_p;
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