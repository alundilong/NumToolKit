#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    explicit Form(MainWindow *mw, QWidget *parent = 0);
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
