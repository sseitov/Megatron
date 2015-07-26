#ifndef CONTROLLERWIDGET_H
#define CONTROLLERWIDGET_H

#include <QWidget>
#include <QTimer>
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
    QTimer *m_joystickTimer;
#ifdef Q_OS_LINUX
    QCAN    *m_can;
#else
    void    *m_can;
#endif

private slots:
    void updateJoystickData();
    void canInitialized(const QString&);
};

#endif // CONTROLLERWIDGET_H
