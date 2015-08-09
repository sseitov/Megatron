#ifndef CONTROLLERWIDGET_H
#define CONTROLLERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QCheckBox>
#include "qjoystick.h"
#include "qcan.h"
#include "sinewidget.h"

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
    QCAN    m_can[2];
    QTimer *m_updateTimer;

    QVector<QCheckBox*> m_dataIndicator2057;
    QVector<QCheckBox*> m_dataIndicator2088PO;
    QVector<SineWidget*> m_dataMonitor2088PO;

private slots:
    void updateData();
    void canInitialized(QCAN*);

    void getLowCAN2057value(unsigned int);
    void getHighCAN2057value(unsigned int);
    void getPWMDuty(int, int);
};

#endif // CONTROLLERWIDGET_H
