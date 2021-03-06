#ifndef CANCONTROL_H
#define CANCONTROL_H

#include <QObject>
#include <QCheckBox>
#include <QLCDNumber>
#include <QGroupBox>
#include <QProgressBar>
#include <QSlider>
#include <QSpinBox>
#include "qcan.h"

#define PWM_COUNT 6

class CANControl : public QObject
{
    Q_OBJECT
public:
    explicit CANControl(QObject *parent = 0);

    void init(int node);
    void start();
    void stop();

    QCan *mCan;
    int mNode;
    QGroupBox *mBox;
    QVector<QProgressBar*> mOutputPulseIndicator;
    QCheckBox *mInversion;
    QCheckBox *mButtons[2];
    QSlider    *mFrequency;
    QLCDNumber *mFrequiencyIndicator;
    QVector<QSpinBox*> hiLimit;
    QVector<QSpinBox*> loLimit;

    void setValue(int port, int value);

public slots:
    void setDuty(int value);
    void setFrequency(int value);
    void setInversion(bool);

private:
    int mDuty[8];
    int mOriginDuty[8];

};

#endif // CANCONTROL_H
