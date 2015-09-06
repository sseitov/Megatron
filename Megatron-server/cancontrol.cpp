#include "cancontrol.h"

CANControl::CANControl(QObject *parent) :
    QObject(parent), mNode(-1)
{
}

void CANControl::start(int node)
{
    mNode = node;
    for (int i=0; i<PWM_COUNT; i++) {
        connect(mOutputPulseIndicator[i], SIGNAL(valueChanged(int)), this, SLOT(setDuty(int)));
    }
    connect(mFrequency, SIGNAL(valueChanged(int)), this, SLOT(setFrequency(int)));
}

void CANControl::setFrequency(int value)
{
    if (mNode < 0) return;
    mFrequiencyIndicator->display(value/10000);
    mCan->setPulseFrequency(mNode, value);
}

void CANControl::setDuty(int value)
{
    if (mNode < 0) return;
    int port = sender()->objectName().toInt();
    mCan->setPulseDuty(mNode, port, value, mInversion->isChecked());
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
        mCan->setPulseOutput(mNode, i, false);
        mOutputPulseIndicator[i]->setValue(0);
    }
}
