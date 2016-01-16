#include "buttonsetup.h"
#include "ui_buttonsetup.h"

ButtonSetup::ButtonSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ButtonSetup)
{
    ui->setupUi(this);
}

ButtonSetup::~ButtonSetup()
{
    delete ui;
}

void ButtonSetup::setupButton(ControlButton* button)
{
    ButtonConfig config;
    config.name = ui->name->text();
    config.checkable = ui->fixation->isChecked();
    config.inverse = ui->inverse->isChecked();
    config.canID = ui->canID->currentText().toInt();
    config.port = ui->output->currentIndex();
    button->setConfig(config);
}
