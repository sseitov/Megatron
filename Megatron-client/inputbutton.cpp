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
    mInverse = false;
    mPort = 0;
}

void InputButton::configure(const QString& name, bool checkable, bool inverse, int port)
{
    setStyleSheet("QGroupBox { color : black; }");
    setTitle(name);
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
        mButton->setStyleSheet("QPushButton { background : black; }");
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
        mButton->setStyleSheet("QPushButton { background : black; }");
    } else {
        mButton->setStyleSheet("QPushButton { background : white; }");
    }
    emit setLevel(mPort, checked);
}
