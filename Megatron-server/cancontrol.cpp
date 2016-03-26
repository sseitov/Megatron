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

void CANControl::init(int node)
{
    mNode = node;
    if (mNode < 0) return;
    int value = mInversion->isChecked() ? 999 : 1;
    for (int i=0; i<8; i++) {
        mCan->setPulseFrequency(mNode, i, mFrequency->value()*1000);
        mCan->setPulseDuty(mNode, i, value);
    }
}

void CANControl::start()
{
    if (mNode >= 0) {
        for (int i=0; i<8; i++) {
            mCan->setPulseOutput(mNode, i, true, mInversion->isChecked());
        }
    }
}

void CANControl::stop()
{
    if (mNode >= 0) {
        for (int i=0; i<8; i++) {
            mCan->setPulseOutput(mNode, i, false, mInversion->isChecked());
        }
    }
}

void CANControl::setFrequency(int value)
{
    double floatValue = double(value)/10.0;
    mFrequiencyIndicator->display(floatValue);
    if (mNode < 0) return;
    for (int i=0; i<8; i++) {
        mCan->setPulseFrequency(mNode, i, value*1000);
    }
}

void CANControl::setDuty(int value)
{
    int port = sender()->objectName().toInt();
    if (mNode < 0) {
        return;
    }

    mOriginDuty[port] = value;

    int hi = hiLimit[port]->value();
    int lo = loLimit[port]->value();

    value = lo + (hi-lo)*value/1000;

    if (mInversion->isChecked())
        value = 1000 - value;
    if (mDuty[port] != value) {
        mCan->setPulseDuty(mNode, port, value);
    }
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
            value = (value > 1) ? hiLimit[6]->value() : loLimit[6]->value();
            mCan->setPulseDuty(mNode, 6, value);
        } else if (port == 7) {
            mButtons[1]->setChecked(value > 1);
            value = (value > 1) ? hiLimit[7]->value() : loLimit[7]->value();
            mCan->setPulseDuty(mNode, 7, value);
        }
    }
}
