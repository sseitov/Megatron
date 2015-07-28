#include "controllerwidget.h"
#include "ui_controllerwidget.h"
#include <QDebug>
#include <QMessageBox>

ControllerWidget::ControllerWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ControllerWidget)
{
    m_ui->setupUi(this);
    m_dataIndicator.push_back(m_ui->d0);
    m_dataIndicator.push_back(m_ui->d1);
    m_dataIndicator.push_back(m_ui->d2);
    m_dataIndicator.push_back(m_ui->d3);
    m_dataIndicator.push_back(m_ui->d4);
    m_dataIndicator.push_back(m_ui->d5);
    m_dataIndicator.push_back(m_ui->d6);
    m_dataIndicator.push_back(m_ui->d7);
    m_dataIndicator.push_back(m_ui->d8);
    m_dataIndicator.push_back(m_ui->d9);
    m_dataIndicator.push_back(m_ui->d10);
    m_dataIndicator.push_back(m_ui->d11);
    m_dataIndicator.push_back(m_ui->d12);
    m_dataIndicator.push_back(m_ui->d13);
    m_dataIndicator.push_back(m_ui->d14);
    m_dataIndicator.push_back(m_ui->d15);

    m_ui->monitor->setStyleSheet("background-color:black;");

    m_joystick = new QJoystick();
    if (m_joystick->init()) {
        m_ui->joystick->setTitle(m_joystick->joystickName());
    } else {
        m_ui->joystick->setTitle("Joystick not found");
    }

    m_can = new QCAN();
    if (m_can->init()) {
        connect(m_can, SIGNAL(initialized(const QString&)), this, SLOT(canInitialized(const QString&)));
        connect(m_can, SIGNAL(lowValues(unsigned int)), this, SLOT(getLowCANvalue(unsigned int)));
        connect(m_can, SIGNAL(highValues(unsigned int)), this, SLOT(getHighCANvalue(unsigned int)));
        connect(m_ui->yAxis, SIGNAL(valueChanged(int)), this, SLOT(setCANvalue(int)));
    } else {
        m_ui->can->setTitle("CAN not found");
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
    delete m_can;
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

void ControllerWidget::canInitialized(const QString& name)
{
    m_ui->can->setTitle(name);
}

void ControllerWidget::setCANvalue(int value)
{
    m_can->writeValue(value);
}

void ControllerWidget::getLowCANvalue(unsigned int value)
{
    qDebug() << "LOW " << value;
    int offset = 0x1;
    for (int i=0; i<8; i++) {
        if (value & (offset << i)) {
            if (!m_dataIndicator[i]->isChecked()) {
                m_dataIndicator[i]->setChecked(true);
            }
        } else {
            if (m_dataIndicator[i]->isChecked()) {
                m_dataIndicator[i]->setChecked(false);
            }
        }
    }
}

void ControllerWidget::getHighCANvalue(unsigned int value)
{
//    qDebug() << "HIGH " << value;
    int offset = 0x1;
    for (int i=0; i<8; i++) {
        if (value & (offset << i)) {
            if (!m_dataIndicator[i+8]->isChecked()) {
                m_dataIndicator[i+8]->setChecked(true);
            }
        } else {
            if (m_dataIndicator[i+8]->isChecked()) {
                m_dataIndicator[i+8]->setChecked(false);
            }
        }
    }
}
