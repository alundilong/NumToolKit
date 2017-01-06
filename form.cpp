#include "form.h"
#include "ui_form.h"
#include <QMessageBox>
#include <iostream>
#include "linearalgebrasolver.h"
#include "mainwindow.h"

using namespace std;

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->spinBox->setValue(1);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(1);
    QStringList tbHeader;
    tbHeader << "A1" << "b";
    ui->tableWidget->setHorizontalHeaderLabels(tbHeader);

//    connect(ui->pushButton,SIGNAL(clicked()),this, SLOT(exit()));
//    connect(ui->solveButton,SIGNAL(clicked()),this,SLOT(solve()));

}

Form::Form(MainWindow *mw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    mw(mw)
{
    ui->setupUi(this);
    ui->spinBox->setValue(1);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(1);
    QStringList tbHeader;
    tbHeader << "A1" << "b";
    ui->tableWidget->setHorizontalHeaderLabels(tbHeader);
}

Form::~Form()
{
    delete ui;
}

// quit button
void Form::on_pushButton_clicked()
{
    exit();
}

void Form::exit() {
    this->exit();
}

/*
void Form::on_pushButton_2_clicked()
{
    // solve the linear algebra equations
    solve();
}

*/

void Form::solve() {

    int ncol = nc_ + 1;
    int nrow = nl_;

    double **A = new double*[nrow];
    for (int i = 0; i < nrow; i++) {
        A[i] = new double [ncol];
    }
    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol-1; j++) {
            A[i][j] = ui->tableWidget->item(i,j)->text().toDouble();
//            cout << i <<"\t"<< j <<"\t"<< A[i][j] << endl;
        }
    }

    double *bValues = new double[nl_];
    for (int i = 0; i < nrow; i++) {
        bValues[i] = ui->tableWidget->item(i,ncol-1)->text().toDouble();
//        cout << i <<"\t"<< ncol-1 << "\t" << bValues[i] << endl;
    }

// call linearAlgebraClass
    double *results = new double[nrow];
    linearAlgebraSolver las (nrow,A,bValues,results);
    las.GaussElimination();

    QString string;
//    string += QString::number(nrow) + "\n";
//    string += QString::number(ncol) + "\n";
//    string += "Solution is :\n";
//    for (int i = 0; i < nrow; i++) {
//        string += QString::number(results[i]) + "\n";
//    }

//    for (int i = 0; i < nrow; i++) {
//        for (int j = 0; j < ncol; j++) {
//            string += ui->tableWidget->item(i,j)->text() + " ";
//        }
//        string += "\n";
//    }

    string = "This is a " + QString::number(nrow) +" x " \
          + QString::number(ncol-1) + " matrix \n";
    string += "Start computing ...\n";
    string += "Result is \n";
    for (int i = 0; i < nrow; i++) {
        string += QString::number(results[i]) + "\n";
    }

    ui->textBrowser->setText(string);
    log_ += string;

    mw->retrieveLogFromLAS();

//    ((MainWindow*)parentWidget())->retrieveLogFromLAS();

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
    int ncol = nc_ + 1;
    int nrow = nl_;
    ui->tableWidget->setColumnCount(ncol);
    ui->tableWidget->setRowCount(nrow);
    QStringList tbHeader;
    for (int i = 0; i < ncol-1; i++) {
        tbHeader << QString("A")+QString::number(i+1);
    }
    tbHeader << "b";
    ui->tableWidget->setHorizontalHeaderLabels(tbHeader);
}
