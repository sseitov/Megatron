#ifndef CANCONTROL_H
#define CANCONTROL_H

#include <QObject>
#include <QCheckBox>
#include <QLCDNumber>
#include <QGroupBox>
#include <QProgressBar>
#include <QSlider>
#include "qcan.h"

#define PWM_COUNT 8

class CANControl : public QObject
{
    Q_OBJECT
public:
    explicit CANControl(QObject *parent = 0);

    void start(int node);

    QCan *mCan;
    int mNode;
    QGroupBox *mBox;
    QVector<QProgressBar*> mOutputPulseIndicator;
    QCheckBox *mInversion;
    QSlider *mFrequency;
    QLCDNumber *mFrequiencyIndicator;

    void setValue(int port, int value);
    void set();
    void reset();

signals:

public slots:
    void setDuty(int value);
    void setFrequency(int value);

};

#endif // CANCONTROL_H
