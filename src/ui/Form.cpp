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

#include "Form.h"
#include "ui_form.h"
#include <QMessageBox>
#include "../fea/math/LinearAlgebraSolver.h"
#include "MainWindow.h"
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

Form::Form(int size, double **A, double *b, MainWindow *mw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    mw(mw)
{
    ui->setupUi(this);
    ui->spinBox->setValue(size);
    ui->tableWidget->setColumnCount(size+2);
    ui->tableWidget->setRowCount(size);

    QStringList tbHeader;
    for (int i = 0; i < size; i++) {
        tbHeader << QString("A") + QString::number(i+1);
    }
    tbHeader << "b" << "x";
    ui->tableWidget->setHorizontalHeaderLabels(tbHeader);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(A[i][j])));
        }
        ui->tableWidget->setItem(i, size, new QTableWidgetItem(QString::number(b[i])));
    }

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

    mathExtension::Matrix A(nrow, nrow);

    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol-2; j++) {
            A[i][j] = ui->tableWidget->item(i,j)->text().toDouble();
        }
    }

    mathExtension::Vector bValues(nrow);
    for (int i = 0; i < nrow; i++) {
        bValues[i] = ui->tableWidget->item(i,ncol-2)->text().toDouble();
    }

    // call linearAlgebraClass
    mathExtension::Vector results(nrow);
    linearAlgebraSolver las (A, bValues, results);

    QString string;
    string = "\nStarting new computing...\n";
    string += "A is a " + QString::number(nrow) +" x " \
          + QString::number(ncol-2) + " matrix \n";

//    std::cout << A << std::endl;
//    std::cout << bValues << std::endl;

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
