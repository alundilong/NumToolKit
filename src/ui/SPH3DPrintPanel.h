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

#ifndef SPH3DPRINTPANEL_H
#define SPH3DPRINTPANEL_H

#include <QWidget>
#include "ViewerWindow.h"

namespace Ui {
class SPH3DPrintPanel;
}

class MainWindow;

class SPH3DPrintPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SPH3DPrintPanel(QWidget *parent = 0);
    explicit SPH3DPrintPanel(MainWindow *mw, ViewerWindow *vwin, QWidget *parent = 0);
    ~SPH3DPrintPanel();

signals:
    void stlmeshLoaded();

private slots:
    void on_loadStlMeshBotton_clicked();

    void on_dumpButton_clicked();

    void on_SetPathToInFile_clicked();

    void on_setPathToExe_clicked();

    void on_printButton_clicked();

private:
    Ui::SPH3DPrintPanel *ui;
    MainWindow *mw_;
    ViewerWindow *vwin_;
    STLMesh *stlmesh_;
    QString path2ExE_;
    QList<QVector3D> sphParticles_;

    inline const STLMesh* stlmesh() { return stlmesh_; }
    inline const QString path2ExE() { return path2ExE_;}
    inline const QList<QVector3D> sphParticles() { return sphParticles_; }
};

#endif // SPH3DPRINTPANEL_H
