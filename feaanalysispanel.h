#ifndef FEAANALYSISPANEL_H
#define FEAANALYSISPANEL_H

#include <QWidget>

namespace Ui {
class feaAnalysisPanel;
}

class feaAnalysisPanel : public QWidget
{
    Q_OBJECT

public:
    explicit feaAnalysisPanel(QWidget *parent = 0);
    ~feaAnalysisPanel();

private slots:
    void on_xRotSlider_sliderMoved(int position);

    void on_yRotSlider_sliderMoved(int position);

    void on_zRotSlider_sliderMoved(int position);

private:
    Ui::feaAnalysisPanel *ui;
};

#endif // FEAANALYSISPANEL_H
