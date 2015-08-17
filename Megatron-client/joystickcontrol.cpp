#include "joystickcontrol.h"

JoystickControl::JoystickControl(QWidget *parent) :
    QGroupBox(parent)
{
}

void JoystickControl::connectControls(QSlider *lc, QLCDNumber *li, QSlider *hc, QLCDNumber *hi, JoystickMonitor *monitor)
{
    mLowLimit = lc;
    mLowLimitIndicator = li;
    connect(mLowLimit, SIGNAL(valueChanged(int)), this, SLOT(setLowLimit(int)));

    mHighLimit = hc;
    mHighLimitIndicator = hi;
    connect(mHighLimit, SIGNAL(valueChanged(int)), this, SLOT(setHighLimit(int)));

    mMonitor = monitor;
}

void JoystickControl::setLowLimit(int value)
{
    mLowLimitIndicator->display(value);
}

void JoystickControl::setHighLimit(int value)
{
    mHighLimitIndicator->display(value);
}
