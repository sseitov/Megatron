#include "client.h"
#include "ui_client.h"
#include "buttonsetup.h"
#include "../common.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    ui->joystick->connectControls(ui->frequency, ui->frequencyIndicator, ui->lowLimit, ui->lowLimitIndicator, ui->highLimit, ui->highLimitIndicator, ui->joystickMonitor);
    connect(ui->joystickMonitor, SIGNAL(setLevel(int,int,int,int)), this, SLOT(setLevel(int,int,int,int)));

    ui->connectButton->setText("Connect");
    ui->connectButton->setStyleSheet("background-color:green; color: white;");
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(start(bool)));

    ui->control0->connectButton(ui->button0);
    mInputButton.append(ui->control0);
    ui->control1->connectButton(ui->button1);
    mInputButton.append(ui->control1);
    ui->control2->connectButton(ui->button2);
    mInputButton.append(ui->control2);
    ui->control3->connectButton(ui->button3);
    mInputButton.append(ui->control3);
    ui->control4->connectButton(ui->button4);
    mInputButton.append(ui->control4);
    ui->control5->connectButton(ui->button5);
    mInputButton.append(ui->control5);
    ui->control6->connectButton(ui->button6);
    mInputButton.append(ui->control6);
    ui->control7->connectButton(ui->button7);
    mInputButton.append(ui->control7);
    ui->control8->connectButton(ui->button8);
    mInputButton.append(ui->control8);
    ui->control9->connectButton(ui->button9);
    mInputButton.append(ui->control9);
    ui->control10->connectButton(ui->button10);
    mInputButton.append(ui->control10);
    ui->control11->connectButton(ui->button11);
    mInputButton.append(ui->control11);
    ui->control12->connectButton(ui->button12);
    mInputButton.append(ui->control12);
    ui->control13->connectButton(ui->button13);
    mInputButton.append(ui->control13);
    ui->control14->connectButton(ui->button14);
    mInputButton.append(ui->control14);
    ui->control15->connectButton(ui->button15);
    mInputButton.append(ui->control15);

    for (int i=0; i<16; i++) {
        mInputButton[i]->setObjectName(QString::number(i));
        connect(mInputButton[i], SIGNAL(toggled(bool)), this, SLOT(connectInput(bool)));
        connect(mInputButton[i], SIGNAL(setLevel(int,bool)), this, SLOT(setLevel(int,bool)));
    }

    mOutputIndicator.append(ui->d0);
    mOutputIndicator.append(ui->d1);
    mOutputIndicator.append(ui->d2);
    mOutputIndicator.append(ui->d3);
    mOutputIndicator.append(ui->d4);
    mOutputIndicator.append(ui->d5);
    mOutputIndicator.append(ui->d6);
    mOutputIndicator.append(ui->d7);
    mOutputIndicator.append(ui->d8);
    mOutputIndicator.append(ui->d9);
    mOutputIndicator.append(ui->d10);
    mOutputIndicator.append(ui->d11);
    mOutputIndicator.append(ui->d12);
    mOutputIndicator.append(ui->d13);
    mOutputIndicator.append(ui->d14);
    mOutputIndicator.append(ui->d15);

    connect(&mServer, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(&mServer, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(&mServer, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(&mServer, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));
}

Client::~Client()
{
    delete ui;
}

void Client::connectInput(bool enabled)
{
    InputButton* control = reinterpret_cast<InputButton*>(sender());
    if (control) {
        if (enabled) {
            ButtonSetup *setup = new ButtonSetup();
            if (setup->exec() == QDialog::Rejected) {
                control->setChecked(false);
            } else {
                setup->setupButton(control);
            }
        } else {
            control->resetConfig();
        }
    }
}

void Client::setLevel(int port, bool value)
{
    mOutputIndicator[port]->setChecked(value);
    if (mServer.isOpen()) {
        QVariantMap map;
        map.insert("CANType", CAN_2057);
        map.insert("CommandType", CAN_SetValue);
        map.insert("Port", port);
        map.insert("Value", value);
        QJsonObject command = QJsonObject::fromVariantMap(map);
        QByteArray data = QJsonDocument(command).toBinaryData();
        mServer.write(data);
    }
}

void Client::setLevel(int port0, int level0, int port1, int level1)
{
    if (mServer.isOpen()) {
        QVariantMap map;
        map.insert("CANType", CAN_2088);
        map.insert("CommandType", CAN_SetValue);

        QVariantList list;

        QVariantMap p0;
        p0.insert("Port", port0);
        p0.insert("Level", level0);
        list.append(p0);

        QVariantMap p1;
        p1.insert("Port", port1);
        p1.insert("Level", level1);
        list.append(p1);

        map.insert("PortArray", list);

        QJsonObject command = QJsonObject::fromVariantMap(map);
        QByteArray data = QJsonDocument(command).toBinaryData();
        mServer.write(data);
    }
}

void Client::start(bool start)
{
    if (start) {
        if (mServer.isOpen()) {
            mServer.close();
        }
        mServer.connectToHost(ui->ipAddress->text(), SERVER_SOCKET);
    } else {
        if (mServer.isOpen()) {
            mServer.close();
        }
    }
}

void Client::onSokReadyRead()
{
    QTcpSocket *server = reinterpret_cast<QTcpSocket*>(sender());
    if (server) {
        QByteArray data = server->readAll();
        QJsonDocument request = QJsonDocument::fromBinaryData(data);
        QJsonObject command = request.object();
        QJsonValue commandType = command.take("CommandType");
        if (commandType == QJsonValue::Undefined)
            return;
        if (commandType.toInt() == CAN_Initialized) {
            QJsonValue canTypes = command.take("CANType");
            if (canTypes == QJsonValue::Undefined)
                return;
            QJsonArray types = canTypes.toArray();
            for (int i=0; i<types.count(); i++) {
                QJsonValue type = types[i];
                if (type.toInt() == CAN_2057) {
                    ui->can_2057->setChecked(true);
                }
                if (type.toInt() == CAN_2088) {
                    ui->joystick->setChecked(true);
                }
            }
        }
    }
}

void Client::onSokConnected()
{
    ui->connectButton->setText("Disconnect");
    ui->connectButton->setStyleSheet("background-color:red; color: white;");
}

void Client::onSokDisconnected()
{
    ui->can_2057->setChecked(false);
    ui->connectButton->setChecked(false);
    ui->connectButton->setText("Connect");
    ui->connectButton->setStyleSheet("background-color:green; color: white;");
}

void Client::onSokDisplayError(QAbstractSocket::SocketError)
{
    QTcpSocket *server = reinterpret_cast<QTcpSocket*>(sender());
    if (server) {
        QMessageBox::critical(0, "Connection Error!", server->errorString(), QMessageBox::Ok);
        server->close();
        onSokDisconnected();
    }
}
