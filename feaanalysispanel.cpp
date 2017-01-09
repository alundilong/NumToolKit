#include "feaanalysispanel.h"
#include "ui_feaanalysispanel.h"
#include "mainwindow.h"
#include "linearalgebrasolver.h"

#include "elementstyle.h"

#include <QDebug>
#include "form.h"

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
//    mw->retrieveLogFromFEAWindow();
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
    mw->retrieveLogFromFEAWindow();

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
        BarElement *barEle = new BarElement(*m,*g);
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
    mw->retrieveLogFromFEAWindow();

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
