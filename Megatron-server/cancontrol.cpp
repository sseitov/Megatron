#include "cancontrol.h"
#include <QDebug>

CANControl::CANControl(QObject *parent) :
    QObject(parent), mNode(-1)
{
    for (int i=0; i<PWM_COUNT; i++) {
        mOriginDuty[i] = 0;
        mDuty[i] = 0;
    }
}

void CANControl::start(int node)
{
    mNode = node;
    if (mNode < 0) return;
    for (int i=0; i<PWM_COUNT; i++) {
        mCan->setPulseFrequency(mNode, i, mFrequency->value()/10);
    }
}

void CANControl::setFrequency(int value)
{
    mFrequiencyIndicator->display(value/10000);
    if (mNode < 0) return;
    for (int i=0; i<PWM_COUNT; i++) {
        mCan->setPulseFrequency(mNode, i, value/10);
    }
}

void CANControl::setDuty(int value)
{
    int port = sender()->objectName().toInt();
    if (mNode < 0) {
//        qDebug() << "set port " << port << " to value " << value;
        return;
    }
    mOriginDuty[port] = value;

    int hi = hiLimit[port]->value();
    int lo = loLimit[port]->value();

    value = lo + (hi-lo)*value/1000;

    if (mInversion->isChecked())
        value = 1000 - value;
    if (mDuty[port] != value)
        mCan->setPulseDuty(mNode, port, value);
    mDuty[port] = value;
}

void CANControl::setInversion(bool)
{
    if (mNode < 0) return;
    for (int i=0; i<PWM_COUNT; i++) {
        int hi = hiLimit[i]->value();
        int lo = loLimit[i]->value();
        int value = mOriginDuty[i];
        value = lo + (hi-lo)*value/1000;

        if (mInversion->isChecked())
            value = 1000 - value;
        if (mDuty[i] != value)
            mCan->setPulseDuty(mNode, i, value);
        mDuty[i] = value;
    }
}

void CANControl::setValue(int port, int value)
{
    if (mNode < 0) return;
    if (port < PWM_COUNT) {
        mOutputPulseIndicator[port]->setValue(value);
    } else {
        if (mInversion->isChecked()) {
            value = 1000 - value;
        }
        if (port == 6) {
            mButtons[0]->setChecked(value > 1);
            mCan->setPulseDuty(mNode, 6, value);
        } else if (port == 7) {
            mButtons[1]->setChecked(value > 1);
            mCan->setPulseDuty(mNode, 7, value);
        }
    }
}


void CANControl::set()
{
    if (mNode < 0) return;
    for (int i=0; i<PWM_COUNT; i++) {
        mCan->setPulseOutput(mNode, i, mInversion->isChecked());
        mOutputPulseIndicator[i]->setValue(0);
    }
    mCan->setPulseOutput(mNode, 6, mInversion->isChecked());
    mCan->setPulseOutput(mNode, 7, mInversion->isChecked());
}

void CANControl::reset()
{
    if (mNode < 0) return;
    for (int i=0; i<PWM_COUNT; i++) {
        mDuty[i] = 0;
        mOriginDuty[i] = 0;
        mCan->setPulseOutput(mNode, i, mInversion->isChecked());
        mOutputPulseIndicator[i]->setValue(0);
    }
}
