#include "joystickcontrol.h"

JoystickControl::JoystickControl(QWidget *parent) :
    QGroupBox(parent)
{
}

void JoystickControl::connectControls(QSlider *fc, QLCDNumber *fi, QSlider *lc, QLCDNumber *li, QSlider *hc, QLCDNumber *hi, JoystickMonitor *monitor)
{
    mFrequencyIndicator = fi;
    mFrequency = fc;
    connect(mFrequency, SIGNAL(valueChanged(int)), this, SLOT(setFrequency(int)));
    
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
    emit sendLowLimit(value);
}

void JoystickControl::setHighLimit(int value)
{
    mHighLimitIndicator->display(value);
    emit sendHighLimit(value);
}

void JoystickControl::setFrequency(int frequency)
{
    mFrequencyIndicator->display(frequency/10000);
    emit sendFrequency(frequency);
}
