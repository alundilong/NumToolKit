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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ViewerWindow.h"
#include "FeaAnalysisPanel.h"
#include "SPH3DPrintPanel.h"

namespace Ui {
class MainWindow;
}

class Form;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_actionUndo_triggered();

    void on_actionAbout_notepad_triggered();

    void on_actionFont_triggered();

    void on_actionColor_triggered();

    void on_actionBC_text_triggered();

    void on_actionBC_text_edit_triggered();

    void on_actionPrint_triggered();

    void on_actionMethod_triggered();

    void on_actionAnalysis_triggered();

    void on_actionSPH_triggered();

public:
    // retrieve log
    void retrieveLogFromMethodWindow();
    void retrieveLogFromFEAWindow();
    QString echo(QString qs);

private:
    Ui::MainWindow *ui;
    QString filePath_;
    QString log_;
    Form * methodWindow_;
    feaAnalysisPanel *fap_;
    SPH3DPrintPanel *sph3DPrint_;
    ViewerWindow * vwinFEA_;
    ViewerWindow * vwinSPH_;

};

#endif // MAINWINDOW_H
