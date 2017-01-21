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

#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "MainWindow.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    explicit Form(MainWindow *mw, QWidget *parent = 0);
    explicit Form(int size, double ** A, double *b, MainWindow *mw, QWidget *parent = 0);
    ~Form();

    const QString & getLog() {
        return log_;
    }

private slots:
//    void on_pushButton_clicked();

//    void on_pushButton_2_clicked();

    void on_solveButton_clicked();

    void on_spinBox_editingFinished();

    void on_radioIterativeMethod_toggled(bool checked);

    void on_radioDirectMethod_toggled(bool checked);

private:
    Ui::Form *ui;
    MainWindow *mw;

    int nl_, nc_; // number of line and column
    QString log_;
//    void exit();

    void solve();

};

#endif // FORM_H
