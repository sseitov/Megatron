#include "joystickcontrol.h"

JoystickControl::JoystickControl(QWidget *parent) :
    QGroupBox(parent)
{
}

void JoystickControl::connectControls(QSlider *fc, QLCDNumber *fi, JoystickMonitor *monitor)
{
    mFrequencyIndicator = fi;
    mFrequency = fc;
    connect(mFrequency, SIGNAL(valueChanged(int)), this, SLOT(setFrequency(int)));

    mMonitor = monitor;
}

void JoystickControl::setFrequency(int frequency)
{
    mFrequencyIndicator->display(frequency/10000);
    emit sendFrequency(frequency);
}
