#include "server.h"
#include "ui_server.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include "../common.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server), mClient(0), mNode2057(0), mNode2088(0)
{
    ui->setupUi(this);
    ui->startButton->setStyleSheet("background-color:green; color: white;");
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(start(bool)));

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
    for (int i=0; i<16; i++) {
        mOutputIndicator[i]->setObjectName(QString::number(i));
    }

    mOutputPulseIndicator.append(ui->po0);
    mOutputPulseIndicator.append(ui->po1);
    mOutputPulseIndicator.append(ui->po2);
    mOutputPulseIndicator.append(ui->po3);
    for (int i=0; i<4; i++) {
        mOutputPulseIndicator[i]->setObjectName(QString::number(i));
    }

    connect(&mServer, SIGNAL(newConnection()), this, SLOT(connection()));
    connect(&mCan, SIGNAL(initialized(int)), this, SLOT(canInitialized(int)));

    if (!mCan.init()) {
        QMessageBox::critical(0, "Startup Error!", "Can not load CAN driver!", QMessageBox::Ok);
    }
}

Server::~Server()
{
    reset2057();
    reset2088();
    delete ui;
}

void Server::start(bool start)
{
    if (start) {
        if (!mServer.listen(QHostAddress::Any, SERVER_SOCKET)) {
            QMessageBox::critical(0, "Unable to start the server", mServer.errorString(), QMessageBox::Ok);
        } else {
            ui->startButton->setText("Stop");
            ui->startButton->setStyleSheet("background-color:red; color: white;");
            QList<QHostAddress> list = QNetworkInterface::allAddresses();
            for(int nIter=0; nIter<list.count(); nIter++) {
                if(!list[nIter].isLoopback()) {
                    if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ) {
                        setWindowTitle(list[nIter].toString());
                        break;
                    }
                }
            }
        }
    } else {
        if (mClient) {
            mClient->close();
            mClient = 0;
        }
        mServer.close();

        ui->startButton->setText("Start");
        ui->startButton->setStyleSheet("background-color:green; color: white;");
        setWindowTitle("Megatron Server");
    }
}

void Server::connection()
{
    if (mClient) {
        mClient->close();
    }
    mClient = mServer.nextPendingConnection();
    connect(mClient, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    connect(mClient, SIGNAL(disconnected()), this, SLOT(slotDisconnectClient()));

    QVariantMap map;
    map.insert("CommandType", CAN_Initialized);
    QVariantList list;
    if (ui->can2057->isEnabled()) {
        list.append(CAN_2057);
    }
    if (ui->can2088->isEnabled()) {
        list.append(CAN_2088);
    }
    map.insert("CANType", list);
    QJsonObject command = QJsonObject::fromVariantMap(map);
    QByteArray data = QJsonDocument(command).toBinaryData();
    mClient->write(data);
}

void Server::slotDisconnectClient()
{
    mClient = 0;
    reset2057();
    reset2088();
}

void Server::slotReadClient()
{
    QTcpSocket *clientSocket = reinterpret_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        QByteArray data = clientSocket->readAll();
        QJsonDocument request = QJsonDocument::fromBinaryData(data);
        QJsonObject command = request.object();
        QJsonValue canType = command.take("CANType");
        if (canType == QJsonValue::Undefined || canType.toInt() == CAN_None)
            return;
        QJsonValue commandType = command.take("CommandType");
        if (commandType == QJsonValue::Undefined)
            return;
        if (commandType.toInt() == CAN_SetValue) {
            if (canType.toInt() == CAN_2057) {
                QJsonValue port = command.take("Port");
                QJsonValue value = command.take("Value");
                set2057Value(port.toInt(), value.toBool());
            } else if (canType.toInt() == CAN_2088) {
                QJsonArray values = command.take("PortArray").toArray();
                for (int i=0; i<values.count(); i++) {
                    QJsonObject value = values[i].toObject();
                    set2088Value(value.take("Port").toInt(), value.take("Level").toInt());
                }
            }
        } else if (commandType.toInt() == CAN_SetPreference) {
            if (canType.toInt() == CAN_2057) {
            } else if (canType.toInt() == CAN_2088) {
                QJsonValue value = command.take("Value");
                ui->frequency->setValue(value.toInt());
            }
        }
    }
}

void Server::canInitialized(int node)
{
    int canType = mCan.getCanType(node);
    if (canType == 0x2088) {
        mNode2088 = node;
        ui->can2088->setEnabled(true);
        for (int i=0; i<4; i++) {
            connect(mOutputPulseIndicator[i], SIGNAL(valueChanged(int)), this, SLOT(set2088duty(int)));
        }
        connect(ui->frequency, SIGNAL(valueChanged(int)), this, SLOT(set2088frequency(int)));
        set2088();
    } else if (canType == 0x2057) {
        mNode2057 = node;
        ui->can2057->setEnabled(true);
        for (int i=0; i<16; i++) {
            connect(mOutputIndicator[i], SIGNAL(stateChanged(int)), this, SLOT(set2057port(int)));
        }
        reset2057();
    }
}

void Server::set2057port(int state)
{
    int port = sender()->objectName().toInt();
    UNS8 value;
    if (port < 8) {
        m2057lowValue.set(port, state > 0 ? 1 : 0);
        value = m2057lowValue.to_ulong();
    } else {
        m2057highValue.set(port-8, state > 0 ? 1 : 0);
        value = m2057highValue.to_ulong();
    }
    mCan.setTrigger(mNode2057, port, value);
}

void Server::set2088frequency(int value)
{
    ui->frequencyIndicator->display(value);
    mCan.setPulseFrequency(mNode2088, value);
}

void Server::set2088duty(int value)
{
    int port = sender()->objectName().toInt();
    mCan.setPulseDuty(mNode2088, port, value);
}

void Server::set2057Value(int port, bool isOn)
{
    mOutputIndicator[port]->setChecked(isOn);
}

void Server::reset2057()
{
    for (int i=0; i<16; i++) {
        mOutputIndicator[i]->setChecked(false);
    }
}

void Server::set2088Value(int port, int value)
{
    mOutputPulseIndicator[port]->setValue(value);
}


void Server::set2088()
{
    for (int i=0; i<4; i++) {
        mCan.setPulseOutput(mNode2088, i, true);
        mOutputPulseIndicator[i]->setValue(0);
    }
}

void Server::reset2088()
{
    for (int i=0; i<4; i++) {
        mCan.setPulseOutput(mNode2088, i, false);
        mOutputPulseIndicator[i]->setValue(0);
    }
}
