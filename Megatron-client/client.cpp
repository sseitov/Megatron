#include "client.h"
#include "ui_client.h"
#include "buttonsetup.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    ui->control0->connectButton(ui->button0);
    mInputButton.append(ui->control0);
    ui->control1->connectButton(ui->button1);
    mInputButton.append(ui->control1);
    ui->control2->connectButton(ui->button2);
    mInputButton.append(ui->control2);
    ui->control3->connectButton(ui->button3);
    mInputButton.append(ui->control3);
    ui->control4->connectButton(ui->button4);
    mInputButton.append(ui->control4);
    ui->control5->connectButton(ui->button5);
    mInputButton.append(ui->control5);
    ui->control6->connectButton(ui->button6);
    mInputButton.append(ui->control6);
    ui->control7->connectButton(ui->button7);
    mInputButton.append(ui->control7);
    ui->control8->connectButton(ui->button8);
    mInputButton.append(ui->control8);
    ui->control9->connectButton(ui->button9);
    mInputButton.append(ui->control9);
    ui->control10->connectButton(ui->button10);
    mInputButton.append(ui->control10);
    ui->control11->connectButton(ui->button11);
    mInputButton.append(ui->control11);
    ui->control12->connectButton(ui->button12);
    mInputButton.append(ui->control12);
    ui->control13->connectButton(ui->button13);
    mInputButton.append(ui->control13);
    ui->control14->connectButton(ui->button14);
    mInputButton.append(ui->control14);
    ui->control15->connectButton(ui->button15);
    mInputButton.append(ui->control15);

    for (int i=0; i<16; i++) {
        mInputButton[i]->setObjectName(QString::number(i));
        connect(mInputButton[i], SIGNAL(toggled(bool)), this, SLOT(connectInput(bool)));
        connect(mInputButton[i], SIGNAL(setLevel(int,bool)), this, SLOT(setLevel(int,bool)));
    }

    mOutputIndicator.append(ui->d0);
    mOutputIndicator.append(ui->d1);
    mOutputIndicator.append(ui->d2);
    mOutputIndicator.append(ui->d3);
    mOutputIndicator.append(ui->d4);
    mOutputIndicator.append(ui->d5);
    mOutputIndicator.append(ui->d6);
    mOutputIndicator.append(ui->d7);
    mOutputIndicator.append(ui->d8);
    mOutputIndicator.append(ui->d9);
    mOutputIndicator.append(ui->d10);
    mOutputIndicator.append(ui->d11);
    mOutputIndicator.append(ui->d12);
    mOutputIndicator.append(ui->d13);
    mOutputIndicator.append(ui->d14);
    mOutputIndicator.append(ui->d15);
}

Client::~Client()
{
    delete ui;
}

void Client::connectInput(bool enabled)
{
    InputButton* control = reinterpret_cast<InputButton*>(sender());
    if (control) {
        if (enabled) {
            ButtonSetup *setup = new ButtonSetup();
            if (setup->exec() == QDialog::Rejected) {
                control->setChecked(false);
            } else {
                setup->setupButton(control);
            }
        } else {
            control->resetConfig();
        }
    }
}

void Client::setLevel(int port, bool value)
{
    mOutputIndicator[port]->setChecked(value);
}
