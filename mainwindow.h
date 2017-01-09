#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

// forward declearation
class Form;
class feaAnalysisPanel;

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
};

#endif // MAINWINDOW_H
