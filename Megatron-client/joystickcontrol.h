#ifndef JOYSTICKCONTROL_H
#define JOYSTICKCONTROL_H

#include <QGroupBox>
#include <QSlider>
#include <QLCDNumber>
#include "joystickmonitor.h"

class JoystickControl : public QGroupBox
{
    Q_OBJECT
public:
    explicit JoystickControl(QWidget *parent = 0);
    void connectControls(QSlider *fc, QLCDNumber *fi, QSlider *lc, QLCDNumber *li, QSlider *hc, QLCDNumber *hi, JoystickMonitor *monitor);

signals:

private slots:
    void setFrequency(int);
    void setLowLimit(int);
    void setHighLimit(int);

private:
    QSlider *mFrequency;
    QLCDNumber *mFrequencyIndicator;
    QSlider *mLowLimit;
    QLCDNumber *mLowLimitIndicator;
    QSlider *mHighLimit;
    QLCDNumber *mHighLimitIndicator;
    JoystickMonitor *mMonitor;
};

#endif // JOYSTICKCONTROL_H
