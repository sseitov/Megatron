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
    ui(new Ui::Server), mServer(0), mClient(0)
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

    mOutputPulseIndicator.append(ui->po0);
    mOutputPulseIndicator.append(ui->po1);
    mOutputPulseIndicator.append(ui->po2);
    mOutputPulseIndicator.append(ui->po3);
}

Server::~Server()
{
    delete ui;
}

void Server::start(bool start)
{
    if (start) {
        mServer = new QTcpServer(this);
        connect(mServer, SIGNAL(newConnection()), this, SLOT(connection()));
        if (!mServer->listen(QHostAddress::Any, SERVER_SOCKET)) {
            QMessageBox::critical(0, "Unable to start the server", mServer->errorString(), QMessageBox::Ok);
        } else {
            ui->can2057->setChecked(true);
            ui->can2088->setChecked(true);
            setWindowTitle(mServer->serverAddress().toString());
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
        if (mServer) {
            mServer->close();
            delete mServer;
            mServer = 0;
        }
        reset2057();
        ui->can2057->setChecked(false);
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
    mClient = mServer->nextPendingConnection();
    connect(mClient, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    connect(mClient, SIGNAL(disconnected()), this, SLOT(slotDisconnectClient()));

    QVariantMap map;
    map.insert("CommandType", CAN_Initialized);
    QVariantList list; list.append(CAN_2057); list.append(CAN_2088);
    map.insert("CANType", list);
    QJsonObject command = QJsonObject::fromVariantMap(map);
    QByteArray data = QJsonDocument(command).toBinaryData();
    mClient->write(data);
}

void Server::slotDisconnectClient()
{
    mClient = 0;
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
        }
    }
}

void Server::set2057Value(int port, bool isOn)
{
    mOutputIndicator[port]->setChecked(isOn);
}

void Server::reset2057()
{
    for (int i=0; i<16; i++) {
        mOutputIndicator[1]->setChecked(false);
    }
}

void Server::set2088Value(int port, int value)
{
    mOutputPulseIndicator[port]->setValue(value);
}
