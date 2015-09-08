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
}

void JoystickControl::setHighLimit(int value)
{
    mHighLimitIndicator->display(value);
}

void JoystickControl::setFrequency(int frequency)
{
    mFrequencyIndicator->display(frequency/10000);
/*
    if (mServer.isOpen()) {
        QVariantMap map;
        map.insert("CANType", CAN_2088);
        map.insert("CommandType", CAN_SetPreference);
        map.insert("Node", 3);
        map.insert("Value", frequency);
        
        QJsonObject command = QJsonObject::fromVariantMap(map);
        QByteArray data = QJsonDocument(command).toBinaryData();
        mServer.write(data);
    }
 */
}
