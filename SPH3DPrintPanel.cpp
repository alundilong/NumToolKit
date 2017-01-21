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

#include "SPH3DPrintPanel.h"
#include "ui_sph3dprintpanel.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "STLMesh.h"

SPH3DPrintPanel::SPH3DPrintPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPH3DPrintPanel)
{
    ui->setupUi(this);
}

SPH3DPrintPanel::SPH3DPrintPanel(MainWindow *mw, ViewerWindow *vwin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPH3DPrintPanel),
    mw_(mw),
    vwin_(vwin),
    stlmesh_(NULL)
{
    ui->setupUi(this);
    connect(this, SIGNAL(stlmeshLoaded()), vwin_, SLOT(updateGL()));
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(destroyed(QObject*)), vwin_, SLOT(close()));
}

SPH3DPrintPanel::~SPH3DPrintPanel()
{
    delete ui;
}

void SPH3DPrintPanel::on_loadStlMeshBotton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open the file");
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(NULL,"..", "file not open");
        return;
    }

    stlmesh_ = new STLMesh(fileName);

    qDebug() << "SPH 3D Printing Panel : number of faces : "  << stlmesh()->nFaces();

    if(stlmesh()->meshState()) {
        vwin_->setFormat(QString("stl"));
        vwin_->setSTLMeshLoadedState(true);
        vwin_->setSTLMesh(stlmesh_);
        emit stlmeshLoaded();
    }
}

void SPH3DPrintPanel::on_dumpButton_clicked()
{
    if(stlmesh()->meshState()) {

        /*
        ITEM: TIMESTEP
        5000
        ITEM: NUMBER OF ATOMS
        234
        ITEM: BOX BOUNDS
        -99.9998 99.9998
        -99.9993 99.9993
        -0.0103184 400.01
        ITEM: ATOMS id type xs ys zs
        */


        QString fileName = QFileDialog::getSaveFileName(this, \
                                                        tr("Save lammpstrj File"), \
                                                        "..", \
                                                        tr("Text Files (*.lammpstrj)"));
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
            //All ok - save data
            QTextStream outstream(&file);

            outstream << QString("ITEM: TIMESTEP\n");
            outstream << QString("0\n");
            outstream << QString("ITEM: NUMBER OF ATOMS\n");
            outstream << QString::number(stlmesh()->nFaces()) + "\n";
            outstream << QString("ITEM: BOX BOUNDS") + "\n";
            outstream << QString::number(stlmesh()->box().xlow) \
                      << "\t" << QString::number(stlmesh()->box().xhig) + "\n";
            outstream << QString::number(stlmesh()->box().ylow) \
                      << "\t" << QString::number(stlmesh()->box().yhig) + "\n";
            outstream << QString::number(stlmesh()->box().zlow) \
                      << "\t" << QString::number(stlmesh()->box().zhig) + "\n";
            outstream << QString("ITEM: ATOMS id type xs ys zs") + "\n";

//            {
                int id = 0;
                int type = 1;
                const QList<QVector3D> & points = stlmesh()->points();
                const QList<QList<int> > & faceNode = stlmesh()->faceNode();

                // vertex -> particle // duplicate points exisit, leave it aside now
                QList<QList<int> >::const_iterator it;
                QStringList line;
                for(it = faceNode.begin(); it != faceNode.end(); ++it) {
                    const QList<int> & nodes = *it;
                    QList<int>::const_iterator it2 = nodes.begin();
                    id++;
                    QVector3D p1(points[(*it2++)]);
                    QVector3D p2(points[(*it2++)]);
                    QVector3D p3(points[(*it2++)]);
                    // face center
                    QVector3D p((p1+p2+p3)/3.0);
                    sphParticles_.push_back(p);
                    line = QStringList();
                    line << QString::number(id) \
                         << QString::number(type) \
                         << QString::number(p.x()) \
                         << QString::number(p.y()) \
                         << QString::number(p.z()) << "\n";
                    outstream << line.join(" ");

//                    if (id > 5) break;
//                }
                // face center -> particle
                // edge center -> particle
            }
            file.close();

        }
    }
}

void SPH3DPrintPanel::on_SetPathToInFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open the file");
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"..", "file not open");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textBrowser->setText(text);
    file.close();
}

void SPH3DPrintPanel::on_setPathToExe_clicked()
{
    path2ExE_ = QFileDialog::getExistingDirectory\
            (this,\
             "Path to lmp_serial",\
             "~/home",\
             QFileDialog::ShowDirsOnly);
}

void SPH3DPrintPanel::on_printButton_clicked()
{
    //QProcess to call lmp_serial

//    QString fileName = path2ExE() + "/lmp_serial";
//    // check if lmp_serial exist
//    QFile file(QFileInfo(fileName).absoluteFilePath());
//    if(file.exists()) {

//    }


    QString fileName = QFileDialog::getSaveFileName(this, \
                                                    tr("Save printing data File"), \
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

        // Slicing the sphParticles to a number of boxes
        int nx = ui->nxSpinBox->value();
        int ny = ui->nySpinBox->value();
        int nz = ui->nzSpinBox->value();
        int ntotal = nx*ny*nz;

        int type = ui->typeSpinBox->value();

        double xlow = stlmesh()->box().xlow;
        double xhig = stlmesh()->box().xhig;
        double ylow = stlmesh()->box().ylow;
        double yhig = stlmesh()->box().yhig;
        double zlow = stlmesh()->box().zlow;
        double zhig = stlmesh()->box().zhig;
        double dx = (xhig-xlow)/nx;
        double dy = (yhig-ylow)/ny;
        double dz = (zhig-zlow)/nz;

        QList<QList<QVector3D> > sphElements;

        // create cell to face, using owner and neighbour
        sphElements.reserve(ntotal);
        for(int i = 0; i < ntotal; i++) {
            QList<QVector3D> qv;
            sphElements.append(qv);
        }

        QList<QVector3D>::const_iterator it;
        for(it = sphParticles().begin(); it != sphParticles().end(); ++it) {

            QVector3D p = *it;
            int id = 0 ;
            int iz = floor((p.z()-zlow)/dz);
            int iy = floor((p.y()-ylow)/dy);
            int ix = floor((p.x()-xlow)/dx);

            id = iz*(nx*ny) + iy*nx + ix;
//            qDebug() << QString("%1 %2 %3 %4\n").arg(ix).arg(iy).arg(iz).arg(id);
            sphElements[id].push_back(p);
        }

        //All ok - save data
        QTextStream outstream(&file);

        QList<QList<QVector3D> >::const_iterator itt;
        for(itt = sphElements.begin(); itt != sphElements.end(); ++itt) {
            QList<QVector3D>::const_iterator itt2;
            QStringList header;

            if((*itt).size() < 1) continue;

            header << "sphElement";
            header << QString::number((*itt).size());
            header << "\n";
            outstream << header.join(" ");

            for(itt2 = (*itt).begin(); itt2 != (*itt).end(); ++itt2) {
                    QStringList data;
                    data << QString::number(type);
                    data << QString::number((*itt2).x());
                    data << QString::number((*itt2).y());
                    data << QString::number((*itt2).z());
                    data << "\n";
                    outstream << data.join(" ");
            }
        }

        file.close();
    }

}
