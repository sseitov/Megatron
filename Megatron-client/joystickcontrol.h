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
    void connectControls(QSlider *fc, QLCDNumber *fi, JoystickMonitor *monitor);

signals:
    void sendFrequency(int frequency);

private slots:
    void setFrequency(int frequency);

private:
    QSlider *mFrequency;
    QLCDNumber *mFrequencyIndicator;
    JoystickMonitor *mMonitor;
};

#endif // JOYSTICKCONTROL_H
