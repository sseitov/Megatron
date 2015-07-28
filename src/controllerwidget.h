#ifndef CONTROLLERWIDGET_H
#define CONTROLLERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QCheckBox>
#include "qjoystick.h"
#ifdef Q_OS_LINUX
#include "qcan.h"
#endif

namespace Ui {
class ControllerWidget;
}

class ControllerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControllerWidget(QWidget *parent = 0);
    ~ControllerWidget();

private:
    Ui::ControllerWidget *m_ui;

    QJoystick *m_joystick;
    QCAN    *m_can;
    QTimer *m_updateTimer;
    QVector<QCheckBox*> m_dataIndicator;

    void resetIndicators();

private slots:
    void updateData();
    void canInitialized(const QString&);
    void setCANvalue(int);
    void getLowCANvalue(int);
    void getHighCANvalue(int);
};

#endif // CONTROLLERWIDGET_H
