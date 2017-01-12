#include "sph3dprintpanel.h"
#include "ui_sph3dprintpanel.h"

SPH3DPrintPanel::SPH3DPrintPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPH3DPrintPanel)
{
    ui->setupUi(this);
}

SPH3DPrintPanel::~SPH3DPrintPanel()
{
    delete ui;
}
