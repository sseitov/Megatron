#include "controllerwidget.h"
#include "ui_controllerwidget.h"
#include <QDebug>
#include <QMessageBox>

/***************************  INITIALISATION  **********************************/

void Init(CO_Data*, UNS32)
{
}

/***************************  CLEANUP  *****************************************/
void Exit(CO_Data*, UNS32)
{
}

ControllerWidget::ControllerWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ControllerWidget)
{
    m_ui->setupUi(this);

    m_dataIndicator2057.push_back(m_ui->d0);
    m_dataIndicator2057.push_back(m_ui->d1);
    m_dataIndicator2057.push_back(m_ui->d2);
    m_dataIndicator2057.push_back(m_ui->d3);
    m_dataIndicator2057.push_back(m_ui->d4);
    m_dataIndicator2057.push_back(m_ui->d5);
    m_dataIndicator2057.push_back(m_ui->d6);
    m_dataIndicator2057.push_back(m_ui->d7);
    m_dataIndicator2057.push_back(m_ui->d8);
    m_dataIndicator2057.push_back(m_ui->d9);
    m_dataIndicator2057.push_back(m_ui->d10);
    m_dataIndicator2057.push_back(m_ui->d11);
    m_dataIndicator2057.push_back(m_ui->d12);
    m_dataIndicator2057.push_back(m_ui->d13);
    m_dataIndicator2057.push_back(m_ui->d14);
    m_dataIndicator2057.push_back(m_ui->d15);

    m_dataIndicator2088PO.push_back(m_ui->po0);
    m_dataIndicator2088PO.push_back(m_ui->po1);
    m_dataIndicator2088PO.push_back(m_ui->po2);
    m_dataIndicator2088PO.push_back(m_ui->po3);
    m_dataIndicator2088PO.push_back(m_ui->po4);
    m_dataIndicator2088PO.push_back(m_ui->po5);
    m_dataIndicator2088PO.push_back(m_ui->po6);
    m_dataIndicator2088PO.push_back(m_ui->po7);

    m_dataMonitor2088PO.push_back(m_ui->P0);
    m_dataMonitor2088PO.push_back(m_ui->P1);
    m_dataMonitor2088PO.push_back(m_ui->P2);
    m_dataMonitor2088PO.push_back(m_ui->P3);
    m_dataMonitor2088PO.push_back(m_ui->P4);
    m_dataMonitor2088PO.push_back(m_ui->P5);
    m_dataMonitor2088PO.push_back(m_ui->P6);
    m_dataMonitor2088PO.push_back(m_ui->P7);

    for (int i=0; i<8; i++) {
        m_dataIndicator2088PO[i]->setObjectName(QString::number(i));
        m_dataMonitor2088PO[i]->setObjectName(QString::number(i));
    }

    m_joystick = new QJoystick();
    if (m_joystick->init()) {
        m_ui->joystick->setTitle(m_joystick->joystickName());
    } else {
        m_ui->joystick->setTitle("Joystick not found");
    }

    // Start CAN Timer thread
    if (LoadCanDriver("/usr/local/lib/libcanfestival_can_vscom.so")) {
        TimerInit();
        StartTimerLoop(&Init);

        if (m_can[0].init("/dev/ttyUSB0", 2)) {
            connect(&m_can[0], SIGNAL(initialized(QCAN*)), this, SLOT(canInitialized(QCAN*)));
        }
    }

    m_updateTimer = new QTimer;
    m_updateTimer->setInterval(50);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    m_updateTimer->start();
}

ControllerWidget::~ControllerWidget()
{
    m_updateTimer->stop();
    delete m_updateTimer;

    // Stop timer thread
    StopTimerLoop(&Exit);
    TimerCleanup();

    delete m_joystick;
    delete m_ui;
}

void ControllerWidget::updateData()
{
    if (m_joystick-> started()) {
        QList<int> axis;
        QList<bool> buttons;
        m_joystick->getData(axis, buttons);

        int x = axis[0];
        int y = axis[1]*(-1);
        int z = axis[2];

        m_ui->xAxis->setValue(x);
        m_ui->yAxis->setValue(y);
        m_ui->zAxis->setValue(z);

        m_ui->button1->setChecked(buttons[0]);
        m_ui->button2->setChecked(buttons[1]);
    }
}

void ControllerWidget::canInitialized(QCAN* can)
{
    int id = can->getID();
    if (id == CAN_2057) {
        m_ui->can->setTitle(can->getTitle());
        connect(can, SIGNAL(low2057Values(unsigned int)), this, SLOT(getLowCAN2057value(unsigned int)));
        connect(can, SIGNAL(high2057Values(unsigned int)), this, SLOT(getHighCAN2057value(unsigned int)));
        connect(m_ui->yAxis, SIGNAL(valueChanged(int)), can, SLOT(writeValue2057(int)));
    } else if (id == CAN_2088) {
        m_ui->can_2->setTitle(can->getTitle());
        connect(m_ui->xAxis, SIGNAL(valueChanged(int)), can, SLOT(writePwmDuty(int)));
        connect(can, SIGNAL(pwmDuty(int, int)), this, SLOT(getPWMDuty(int, int)));
        for (int i=0; i<8; i++) {
            connect(m_dataIndicator2088PO[i], SIGNAL(toggled(bool)), can, SLOT(togglePwmOutput(bool)));
        }
    }
}

void ControllerWidget::getLowCAN2057value(unsigned int value)
{
    int offset = 0x1;
    for (int i=0; i<8; i++) {
        if (value & (offset << i)) {
            if (!m_dataIndicator2057[i]->isChecked()) {
                m_dataIndicator2057[i]->setChecked(true);
            }
        } else {
            if (m_dataIndicator2057[i]->isChecked()) {
                m_dataIndicator2057[i]->setChecked(false);
            }
        }
    }
}

void ControllerWidget::getHighCAN2057value(unsigned int value)
{
    int offset = 0x1;
    for (int i=0; i<8; i++) {
        if (value & (offset << i)) {
            if (!m_dataIndicator2057[i+8]->isChecked()) {
                m_dataIndicator2057[i+8]->setChecked(true);
            }
        } else {
            if (m_dataIndicator2057[i+8]->isChecked()) {
                m_dataIndicator2057[i+8]->setChecked(false);
            }
        }
    }
}

void ControllerWidget::getPWMDuty(int index, int value)
{
    SineWidget *w = m_dataMonitor2088PO[index];
    w->setPulseDuty(value);
}
