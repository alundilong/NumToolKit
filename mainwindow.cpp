#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qlabel.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFontDialog>
#include <QFont>
#include <QColorDialog>
#include <QColor>
#include <QPrintDialog>
#include <QPrinter>
#include "form.h"
#include "feaanalysispanel.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    on_actionNew_triggered();

    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, Qt::blue);
    ui->textEdit->setPalette(p);

    QString welcome = "================= Welcome to NotePad! ================\n";
    ui->textEdit->setText(welcome);

    // ubuntu 14.04 will need this line
    this->menuBar()->setNativeMenuBar(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete iterativeMethodWindow_;
    delete directMethodWindow_;
}


void MainWindow::on_actionNew_triggered()
{
    filePath_ = "";
    ui->textEdit->setText("");
    this->setWindowTitle("new file");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open the file");
    QFile file(fileName);
    filePath_ = fileName;
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"..", "file not open");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = "";
    if (filePath_ == fileName){
        fileName = QFileDialog::getSaveFileName(this,"Save");
    } else {
        fileName = filePath_;
    }
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,"..", "file not save");
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save as");
    filePath_ = fileName;
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,"..", "file not open");
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
     ui->textEdit->paste();
}

void MainWindow::on_actionRedo_triggered()
{
     ui->textEdit->redo();
}

void MainWindow::on_actionUndo_triggered()
{
     ui->textEdit->undo();
}

void MainWindow::on_actionAbout_notepad_triggered()
{
    QString aboutText;
    aboutText  = "Author    : YJ\n";
    aboutText += "Date      : 1/03/2017\n";
    aboutText += "(C) Notepad  (R)";
    QMessageBox::about(this,"about notePad", aboutText);
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,this);

    if(ok) {
        ui->textEdit->setFont(font);
    } else return;
}

void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    if(color.isValid()){
        ui->textEdit->setTextColor(color);
    }

}

void MainWindow::on_actionBC_text_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    if(color.isValid()) {
        ui->textEdit->setTextBackgroundColor(color);
    }
}

void MainWindow::on_actionBC_text_edit_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    if(color.isValid()) {
        ui->textEdit->setPalette(QPalette(color));
    }
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("HPLaserJet P1102W");
    QPrintDialog dialog(&printer, this);
    if(dialog.exec() == QDialog::Rejected) return;
    ui->textEdit->print(&printer);
}

void MainWindow::on_actionDirectMethod_triggered()
{
    directMethodWindow_ = new Form(this);
    directMethodWindow_->setWindowTitle(QString("Direct Method"));
//    directMethodWindow_->setWindowModality(Qt::WindowModal);
    directMethodWindow_->show();
//    connect(directMethodWindow_, SIGNAL(solve()), this, SLOT(retrieveLogFromLAS()));
}

void MainWindow::on_actionIterativeMethod_triggered()
{
    iterativeMethodWindow_ = new Form(this);
    iterativeMethodWindow_->setWindowTitle(QString("Iterative Method"));
    iterativeMethodWindow_->show();
}

void MainWindow::on_action1D_Element_triggered()
{
    fap1D_ = new feaAnalysisPanel(this);
    fap1D_->setWindowTitle("FEA 1D analysis");
    fap1D_->show();
}

void MainWindow::on_action2D_Element_triggered()
{
    fap2D_ = new feaAnalysisPanel(this);
    fap2D_->setWindowTitle("FEA 2D analysis");
    fap2D_->show();
}

void MainWindow::on_action3D_Element_triggered()
{
    fap3D_ = new feaAnalysisPanel(this);
    fap3D_->setWindowTitle("FEA 3D analysis");
    fap3D_->show();
}

void MainWindow::retrieveLogFromLAS()
{
      ui->textEdit->append(directMethodWindow_->getLog());
}

QString MainWindow::echo(QString qs)
{
//    ui->textEdit->setText("data from test\n");
    return qs;
}



