#ifndef CONTROLLERWIDGET_H
#define CONTROLLERWIDGET_H

#include <QWidget>
#include <QTimer>
#include "qjoystick.h"

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

private slots:
    void updateJoystickData();

};

#endif // CONTROLLERWIDGET_H
