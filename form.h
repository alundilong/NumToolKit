#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_solveButton_clicked();

private:
    Ui::Form *ui;
    int nl_, nc_; // number of line and column
    void exit();
    void solve();
};

#endif // FORM_H
