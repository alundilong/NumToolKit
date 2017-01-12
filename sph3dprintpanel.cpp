#include "sph3dprintpanel.h"
#include "ui_sph3dprintpanel.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "stlmesh.h"

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
