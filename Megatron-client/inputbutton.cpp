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
    resetConfig();
}

void InputButton::resetConfig()
{
    setStyleSheet("QGroupBox { color : grey; }");
    setTitle("Connect");
    mButton->setText("");
    mInverse = false;
    mPort = 0;
}

void InputButton::configure(const QString& name, bool checkable, bool inverse, int port)
{
    setStyleSheet("QGroupBox { color : black; }");
    setTitle("Disconnect");
    mButton->setText(name);
    mButton->setCheckable(checkable);
    mButton->setStyleSheet("QPushButton { background : white; }");
    mInverse = inverse;
    mPort = port;
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
    emit setLevel(mPort, checked);
}
