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
    mInverse = inverse;
    mPort = port;
}

void InputButton::buttonClicked(bool checked)
{
    if (mButton->isCheckable()) {
        emit setLevel(mPort, checked);
    }
}

void InputButton::levelOn()
{
    emit setLevel(mPort, true);
}

void InputButton::levelOff()
{
    emit setLevel(mPort, false);
}
