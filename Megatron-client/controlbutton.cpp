#include "controlbutton.h"

ControlButton::ControlButton(QWidget *parent) :
    QPushButton(parent)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
    connect(this, SIGNAL(pressed()), this, SLOT(levelOn()));
    connect(this, SIGNAL(released()), this, SLOT(levelOff()));
}

void ControlButton::setConfig(const ButtonConfig& config)
{
    mConfig = config;
    setText(mConfig.name);
    setCheckable(mConfig.checkable);
    setStyleSheet("QPushButton { background : white; }");
}

void ControlButton::buttonClicked(bool checked)
{
    if (isCheckable()) {
        sendLevel(checked);
    }
}

void ControlButton::levelOn()
{
    if (!isCheckable()) {
        sendLevel(true);
    } else {
        setChecked(false);
        setStyleSheet("background : black; color : white");
    }
}

void ControlButton::levelOff()
{
    if (!isCheckable()) {
        sendLevel(false);
    }
}

void ControlButton::sendLevel(bool checked)
{
    if (checked) {
        setStyleSheet("background-color : black; color : white;");
    } else {
        setStyleSheet("background-color : white; color : black;");
    }
    if (isCheckable()) {
        setChecked(checked);
    }
    emit setLevel(mConfig.port, checked);
}
