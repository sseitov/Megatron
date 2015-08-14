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

void ButtonSetup::setupButton(InputButton* button)
{
    button->configure(ui->name->text(), ui->fixation->isChecked(), ui->inverse->isChecked(), ui->output->currentIndex());
}
