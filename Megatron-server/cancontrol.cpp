#include "cancontrol.h"

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
    for (int i=0; i<PWM_COUNT; i++) {
        connect(mOutputPulseIndicator[i], SIGNAL(valueChanged(int)), this, SLOT(setDuty(int)));
    }
}

void CANControl::setFrequency(int value)
{
    if (mNode < 0) return;
    mFrequiencyIndicator->display(value/10000);
    for (int i=0; i<PWM_COUNT; i++) {
        mCan->setPulseFrequency(mNode, i, value);
    }
}

void CANControl::setDuty(int value)
{
    if (mNode < 0) return;
    int port = sender()->objectName().toInt();

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
    mOutputPulseIndicator[port]->setValue(value);
}


void CANControl::set()
{
    if (mNode < 0) return;
    for (int i=0; i<PWM_COUNT; i++) {
        mCan->setPulseOutput(mNode, i, true);
        mOutputPulseIndicator[i]->setValue(0);
    }
}

void CANControl::reset()
{
    if (mNode < 0) return;
    for (int i=0; i<PWM_COUNT; i++) {
        mDuty[i] = 0;
        mOriginDuty[i] = 0;
        mCan->setPulseOutput(mNode, i, false);
        mOutputPulseIndicator[i]->setValue(0);
    }
}
