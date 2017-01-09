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
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    on_actionNew_triggered();

    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, Qt::black);
    ui->textEdit->setPalette(p);

    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());
    QTextCursor c = ui->textEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(c);

    ui->textEdit->ensureCursorVisible();
    QFont f = ui->textEdit->font();
    f.setPointSize(10);
    ui->textEdit->setFont(f);
    ui->textEdit->setTextColor(Qt::yellow);

    QString welcome = "==================================================\n";
    welcome += "================= Welcome to NumToolKit!  ================\n";
    welcome += "==================================================\n\n";
    welcome += "About NumToolKit :\n";
    welcome += "NumToolKit is an open-source GUI software built with QT 5.7.0\n";
    welcome += "It is developed in a cross-platform fashion, supporing Window7 and Ubuntu\n";
    welcome += "The software will cover Finite Element Analysis (FEA), Finite Volume Method (FVM), and \n";
    welcome += "Smoothed Particle Hydrodynamics (SPH) and more.\n";
    welcome += "It also builds its own linear algebra equation solver as the engine for FEA and FVM.\n\n";
    ui->textEdit->setText(welcome);

    // ubuntu 14.04 will need this line
    this->menuBar()->setNativeMenuBar(false);
//    this->setWindowIcon(QIcon("NumToolKit_icon.png"));

//    connect(this,SIGNAL(QMainWindow::close()),fap_,SLOT(close()));
//    setAttribute(Qt::WA_DeleteOnClose);
}

MainWindow::~MainWindow()
{
    delete ui;
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


void MainWindow::retrieveLogFromMethodWindow()
{
    ui->textEdit->append(methodWindow_->getLog());
}

void MainWindow::retrieveLogFromFEAWindow()
{
    ui->textEdit->append(fap_->getLog());
}

QString MainWindow::echo(QString qs)
{
    return qs;
}


void MainWindow::on_actionMethod_triggered()
{
    methodWindow_ = new Form(this);
    methodWindow_->setWindowTitle(QString("Method Panel"));
    methodWindow_->show();
}

void MainWindow::on_actionAnalysis_triggered()
{
    fap_ = new feaAnalysisPanel(this);
    fap_->setWindowTitle("FEA static analysis");
    fap_->show();
}
