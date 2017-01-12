#ifndef SPH3DPRINTPANEL_H
#define SPH3DPRINTPANEL_H

#include <QWidget>

namespace Ui {
class SPH3DPrintPanel;
}

class SPH3DPrintPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SPH3DPrintPanel(QWidget *parent = 0);
    ~SPH3DPrintPanel();

private:
    Ui::SPH3DPrintPanel *ui;
};

#endif // SPH3DPRINTPANEL_H
