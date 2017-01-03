#include "form.h"
#include "ui_form.h"
#include <QMessageBox>
#include <iostream>
#include "linearalgebrasolver.h"

using namespace std;

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this, SLOT(exit()));
    connect(ui->solveButton,SIGNAL(clicked()),this,SLOT(solve()));
}


Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    exit();
}

void Form::exit() {
    QApplication::exit();
}

/*
void Form::on_pushButton_2_clicked()
{
    // solve the linear algebra equations
    solve();
}

*/

void Form::solve() {

    // read data from textEdit
    QStringList ns = ui->textEdit->toPlainText().split(" ");
    int ntotal;
    nl_ = ns.at(0).toInt();
    nc_ = ns.at(1).toInt();
    ntotal = nl_ + (nl_*nc_) + 2;

//    cout << ns.count() << endl;
//    cout << nl_ << "\t" << nc_ << "\t" << ntotal << endl;

    if((ns.count() != ntotal) || (nl_ != nc_) || (nl_ < 1))
        QMessageBox::warning(this,"warning","format is incorrect!");

    // read the following nl*nc numbers
    double matrixValues[nl_*nc_];
    for (int i = 0; i < nl_*nc_; i++) {
        matrixValues[i] = ns.at(i+2).toDouble();
    }

    double **A = new double*[nl_];
    for (int i = 0; i < nl_; i++) {
        A[i] = new double [nc_];
    }
    for (int k = 0; k < nl_*nc_; k++)
    {
        int irow = k/nl_;
        int icol = k%nl_;
        A[irow][icol] = matrixValues[k];
    }

    double *bValues = new double[nl_];
    for (int i = 0; i < nl_; i++) {
        bValues[i] = ns.at(i+2+nl_*nc_).toDouble();
    }

    double *results = new double[nl_];
    linearAlgebraSolver las (nl_,nc_,A,bValues,results);
    las.LUSolve();
    // call linearAlgebraClass

    QString string;
//    string += QString::number(nl_) + "\n";
//    string += QString::number(nc_) + "\n";
    for (int i = 0; i < nl_; i++) {
        string += QString::number(results[i]) + "\n";

    }
    ui->textBrowser->setText(string);

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
