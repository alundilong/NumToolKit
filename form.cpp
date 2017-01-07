#include "form.h"
#include "ui_form.h"
#include <QMessageBox>
#include "linearalgebrasolver.h"
#include "mainwindow.h"
#include <QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->spinBox->setValue(1);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(1);
    QStringList tbHeader;
    tbHeader << "A1" << "b" <<"x";
    ui->tableWidget->setHorizontalHeaderLabels(tbHeader);

    // default: direct method, gauss elimination
    ui->radioDirectMethod->setChecked(true);
    ui->radioGauss->setChecked(true);
    ui->radioJacobi->setChecked(true);
}

Form::Form(MainWindow *mw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    mw(mw)
{
    ui->setupUi(this);
    ui->spinBox->setValue(1);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(1);
    QStringList tbHeader;
    tbHeader << "A1" << "b" <<"x";
    ui->tableWidget->setHorizontalHeaderLabels(tbHeader);

    // default: direct method, gauss elimination
    ui->radioDirectMethod->setChecked(true);
    ui->radioGauss->setChecked(true);
    ui->radioJacobi->setChecked(true);
}

Form::~Form()
{
    delete ui;
}

void Form::solve() {

    nl_ = ui->spinBox->value();
    nc_ = nl_;
    int ncol = nc_ + 2;
    int nrow = nl_;

    double **A = new double*[nrow];
    for (int i = 0; i < nrow; i++) {
        A[i] = new double [ncol];
    }
    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol-2; j++) {
            A[i][j] = ui->tableWidget->item(i,j)->text().toDouble();
        }
    }

    double *bValues = new double[nl_];
    for (int i = 0; i < nrow; i++) {
        bValues[i] = ui->tableWidget->item(i,ncol-2)->text().toDouble();
    }

    // call linearAlgebraClass
    double *results = new double[nrow];
    linearAlgebraSolver las (nrow,A,bValues,results);

    QString string;
    string = "\nStarting new computing...\n";
    string += "A is a " + QString::number(nrow) +" x " \
          + QString::number(ncol-2) + " matrix \n";

    if (ui->groupBoxDirectMethod->isEnabled()){
        if(ui->radioGauss->isChecked())
        {
            string += "Direct Method : Gauss Elimination Method is applied!\n";
            las.GaussElimination();
        } else if (ui->radioLU->isChecked()) {
            string += "Direct Method : LU Decomposition Method is applied!\n";
            las.LUSolve();
        }
    }

    if(ui->groupBoxIterativeMethod->isEnabled()) {
        if (ui->radioJacobi->isChecked()) {
            string += "Iterative Method : Jacobi Method is applied!\n";
            las.JacobiMethod();
            string += las.mylog();
        } else if (ui->radioGaussSeidel) {
            string += "Iterative Method : Gauss-Seidel Method is applied!\n";
            las.GaussSeidelMethod();
            string += las.mylog();
        }
    }

    // add results into the tablewidget
    for (int i = 0; i < nrow; i++) {
        ui->tableWidget->setItem(i, ncol-1,
                                 new QTableWidgetItem(QString::number(results[i])));
    }

    string += "Result is \n";
    for (int i = 0; i < nrow; i++) {
        string += QString::number(results[i]) + "\n";
    }

    log_ = "";
    log_ += string;
    mw->retrieveLogFromMethodWindow();

    for (int i = 0; i < nl_; i++) {
        delete [] A[i];
    }
    delete [] A;
    delete [] bValues;
    delete [] results;
}

void Form::on_solveButton_clicked()
{
    // solve the linear algebra equations
    solve();
}

void Form::on_spinBox_editingFinished()
{
    nl_ = ui->spinBox->value();
    nc_ = nl_;
    int ncol = nc_ + 2;
    int nrow = nl_;
    ui->tableWidget->setColumnCount(ncol);
    ui->tableWidget->setRowCount(nrow);
    QStringList tbHeader;
    for (int i = 0; i < ncol-2; i++) {
        tbHeader << QString("A") + QString::number(i+1);
    }
    tbHeader << "b" << "x";
    ui->tableWidget->setHorizontalHeaderLabels(tbHeader);
}

void Form::on_radioIterativeMethod_toggled(bool checked)
{
    ui->groupBoxDirectMethod->setEnabled(!checked);
}

void Form::on_radioDirectMethod_toggled(bool checked)
{
    ui->groupBoxIterativeMethod->setEnabled(!checked);
}
