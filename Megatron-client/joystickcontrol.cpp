#include "joystickcontrol.h"

JoystickControl::JoystickControl(QWidget *parent) :
    QGroupBox(parent)
{
}

void JoystickControl::connectControls(QSlider *fc, QLCDNumber *fi, QSlider *lc, QLCDNumber *li, QSlider *hc, QLCDNumber *hi, JoystickMonitor *monitor)
{
    mFrequency = fc;
    mFrequencyIndicator = fi;
    connect(mFrequency, SIGNAL(valueChanged(int)), this, SLOT(setFrequency(int)));

    mLowLimit = lc;
    mLowLimitIndicator = li;
    connect(mLowLimit, SIGNAL(valueChanged(int)), this, SLOT(setLowLimit(int)));

    mHighLimit = hc;
    mHighLimitIndicator = hi;
    connect(mHighLimit, SIGNAL(valueChanged(int)), this, SLOT(setHighLimit(int)));

    mMonitor = monitor;
}

void JoystickControl::setFrequency(int value)
{
    mFrequencyIndicator->display(value);
}

void JoystickControl::setLowLimit(int value)
{
    mLowLimitIndicator->display(value);
}

void JoystickControl::setHighLimit(int value)
{
    mHighLimitIndicator->display(value);
}
