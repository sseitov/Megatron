#include "controllerwidget.h"
#include "ui_controllerwidget.h"
#include <QDebug>
#include <QMessageBox>

ControllerWidget::ControllerWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ControllerWidget)
{
    m_ui->setupUi(this);
    m_ui->monitor->setStyleSheet("background-color:black;");

    m_joystick = new QJoystick();
    if (m_joystick->init()) {
        m_ui->joystick->setTitle(m_joystick->joystickName());
        m_joystickTimer = new QTimer;
        m_joystickTimer->setInterval(50);
        connect(m_joystickTimer, SIGNAL(timeout()), this, SLOT(updateJoystickData()));
        m_joystickTimer->start();
    } else {
        m_joystickTimer = 0;
        m_ui->joystick->setTitle("Joystick not found");
    }
#ifdef Q_OS_LINUX
    m_can = new QCAN();
    if (m_can->init()) {
        m_ui->can->setTitle(m_can->busName());
        m_ui->baudrate->setCurrentIndex((int)m_can->baudRate());
    } else {
        m_ui->can->setTitle("CAN not found");
    }
#else
    m_can = NULL;
    m_ui->can->setTitle("CAN not found");
#endif
    connect(m_ui->baudrate, SIGNAL(activated(int)), this, SLOT(changeBaudRate(int)));
}

ControllerWidget::~ControllerWidget()
{
    if (m_joystickTimer) {
        m_joystickTimer->stop();
        delete m_joystickTimer;
    }
#ifdef Q_OS_LINUX
    delete m_can;
#endif
    delete m_joystick;
    delete m_ui;
}

void ControllerWidget::updateJoystickData()
{
    QList<int> axis;
    QList<bool> buttons;
    m_joystick->getData(axis, buttons);

    m_ui->xAxis->setValue(axis[0]);
    m_ui->yAxis->setValue(axis[1]*(-1));
    m_ui->zAxis->setValue(axis[2]);

    m_ui->button1->setChecked(buttons[0]);
    m_ui->button2->setChecked(buttons[1]);
}

void ControllerWidget::changeBaudRate(int rate)
{
    bool success = false;
#ifdef Q_OS_LINUX
    success = m_can->setBaudRate((CANBaudRate)rate);
#endif
    if (!success && rate != 0) {
        m_ui->baudrate->setCurrentIndex(0);
        QMessageBox::critical(NULL, "Error", "Error change baudrate");
    }
}
