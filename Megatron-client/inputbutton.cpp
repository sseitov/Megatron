#include "inputbutton.h"
#include <QDebug>

InputButton::InputButton(QWidget *parent) :
    QGroupBox(parent)
{
}

void InputButton::connectButton(QPushButton* button)
{
    mButton = button;
    connect(mButton, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
    connect(mButton, SIGNAL(pressed()), this, SLOT(levelOn()));
    connect(mButton, SIGNAL(released()), this, SLOT(levelOff()));
}

void InputButton::uncheck()
{
    if (mConfig.checkable) {
        mButton->setChecked(false);
        mButton->setStyleSheet("background-color : white; color : black;");
    }
}

void InputButton::resetConfig()
{
    setStyleSheet("QGroupBox { color : grey; }");
    setTitle(tr("Подключить"));
    mButton->setText("");
    mConfig.name = "";
    mConfig.checkable = false;
    mConfig.inverse = false;
    mConfig.port = -1;
}

void InputButton::setConfig(const ButtonConfig& config)
{
    mConfig = config;
    if (mConfig.port < 0) {
        setStyleSheet("QGroupBox { color : grey; }");
        setTitle(tr("Подключить"));
        setChecked(false);
    } else {
        setStyleSheet("QGroupBox { color : black; }");
        setTitle(tr("Отключить"));
        setChecked(true);
    }
    mButton->setText(mConfig.name);
    mButton->setCheckable(mConfig.checkable);
    mButton->setStyleSheet("QPushButton { background : white; }");
}

void InputButton::buttonClicked(bool checked)
{
    if (mButton->isCheckable()) {
        sendLevel(checked);
    }
}

void InputButton::levelOn()
{
    if (!mButton->isCheckable()) {
        sendLevel(true);
    } else {
        mButton->setStyleSheet("background : black; color : white");
    }
}

void InputButton::levelOff()
{
    if (!mButton->isCheckable()) {
        sendLevel(false);
    }
}

void InputButton::sendLevel(bool checked)
{
    if (checked) {
        mButton->setStyleSheet("background-color : black; color : white;");
    } else {
        mButton->setStyleSheet("background-color : white; color : black;");
    }
    emit setLevel(mConfig.port, checked);
}
