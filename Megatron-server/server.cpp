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
#include <QSettings>
#include "../common.h"

static bool Alive;

Server::Server(QWidget *parent) :
    QWidget(parent), ui(new Ui::Server), mPingerConnected(false), mPingError(0)
{
    ui->setupUi(this);

    mNode2057[0] = 0;
    mNode2057[1] = 0;

    mOutputIndicator[0].append(ui->d0_1);
    mOutputIndicator[0].append(ui->d1_1);
    mOutputIndicator[0].append(ui->d2_1);
    mOutputIndicator[0].append(ui->d3_1);
    mOutputIndicator[0].append(ui->d4_1);
    mOutputIndicator[0].append(ui->d5_1);
    mOutputIndicator[0].append(ui->d6_1);
    mOutputIndicator[0].append(ui->d7_1);
    mOutputIndicator[0].append(ui->d8_1);
    mOutputIndicator[0].append(ui->d9_1);
    mOutputIndicator[0].append(ui->d10_1);
    mOutputIndicator[0].append(ui->d11_1);
    mOutputIndicator[0].append(ui->d12_1);
    mOutputIndicator[0].append(ui->d13_1);
    mOutputIndicator[0].append(ui->d14_1);
    mOutputIndicator[0].append(ui->d15_1);
    for (int i=0; i<16; i++) {
        mOutputIndicator[0][i]->setObjectName(QString::number(i));
    }

    mOutputIndicator[1].append(ui->d0_2);
    mOutputIndicator[1].append(ui->d1_2);
    mOutputIndicator[1].append(ui->d2_2);
    mOutputIndicator[1].append(ui->d3_2);
    mOutputIndicator[1].append(ui->d4_2);
    mOutputIndicator[1].append(ui->d5_2);
    mOutputIndicator[1].append(ui->d6_2);
    mOutputIndicator[1].append(ui->d7_2);
    mOutputIndicator[1].append(ui->d8_2);
    mOutputIndicator[1].append(ui->d9_2);
    mOutputIndicator[1].append(ui->d10_2);
    mOutputIndicator[1].append(ui->d11_2);
    mOutputIndicator[1].append(ui->d12_2);
    mOutputIndicator[1].append(ui->d13_2);
    mOutputIndicator[1].append(ui->d14_2);
    mOutputIndicator[1].append(ui->d15_2);
    for (int i=0; i<16; i++) {
        mOutputIndicator[1][i]->setObjectName(QString::number(i+16));
    }

    ///////////////////////////////////////////////////////////

    mNode2088[0].mCan = &mCan;
    mNode2088[0].mBox = ui->can2088_1;

    mNode2088[0].mInversion = ui->inversion_1;
    connect(mNode2088[0].mInversion, SIGNAL(toggled(bool)), &mNode2088[0], SLOT(setInversion(bool)));

    mNode2088[0].mButtons[0] = ui->po6_1;
    mNode2088[0].mButtons[1] = ui->po7_1;

    mNode2088[0].mFrequency = ui->frequencySlider_1;
    mNode2088[0].mFrequiencyIndicator = ui->frequencyIndicator_1;
    connect(mNode2088[0].mFrequency, SIGNAL(valueChanged(int)), &mNode2088[0], SLOT(setFrequency(int)));

    mNode2088[0].mOutputPulseIndicator.append(ui->po0_1);
    mNode2088[0].mOutputPulseIndicator.append(ui->po1_1);
    mNode2088[0].mOutputPulseIndicator.append(ui->po2_1);
    mNode2088[0].mOutputPulseIndicator.append(ui->po3_1);
    mNode2088[0].mOutputPulseIndicator.append(ui->po4_1);
    mNode2088[0].mOutputPulseIndicator.append(ui->po5_1);

    mNode2088[0].hiLimit.append(ui->hi0_1);
    mNode2088[0].hiLimit.append(ui->hi1_1);
    mNode2088[0].hiLimit.append(ui->hi2_1);
    mNode2088[0].hiLimit.append(ui->hi3_1);
    mNode2088[0].hiLimit.append(ui->hi4_1);
    mNode2088[0].hiLimit.append(ui->hi5_1);
    mNode2088[0].hiLimit.append(ui->hi6_1);
    mNode2088[0].hiLimit.append(ui->hi7_1);

    mNode2088[0].loLimit.append(ui->lo0_1);
    mNode2088[0].loLimit.append(ui->lo1_1);
    mNode2088[0].loLimit.append(ui->lo2_1);
    mNode2088[0].loLimit.append(ui->lo3_1);
    mNode2088[0].loLimit.append(ui->lo4_1);
    mNode2088[0].loLimit.append(ui->lo5_1);
    mNode2088[0].loLimit.append(ui->lo6_1);
    mNode2088[0].loLimit.append(ui->lo7_1);

    for (int i=0; i<PWM_COUNT; i++) {
       mNode2088[0].mOutputPulseIndicator[i]->setObjectName(QString::number(i));
       mNode2088[0].hiLimit[i]->setObjectName(QString::number(i));
       mNode2088[0].loLimit[i]->setObjectName(QString::number(i));
    }

    ///////////////////////////////////////////////////////////

    mNode2088[1].mCan = &mCan;
    mNode2088[1].mBox = ui->can2088_2;

    mNode2088[1].mInversion = ui->inversion_2;
    connect(mNode2088[1].mInversion, SIGNAL(toggled(bool)), &mNode2088[1], SLOT(setInversion(bool)));

    mNode2088[1].mButtons[0] = ui->po6_2;
    mNode2088[1].mButtons[1] = ui->po7_2;

    mNode2088[1].mFrequency = ui->frequencySlider_2;
    mNode2088[1].mFrequiencyIndicator = ui->frequencyIndicator_2;
    connect(mNode2088[1].mFrequency, SIGNAL(valueChanged(int)), &mNode2088[1], SLOT(setFrequency(int)));

    mNode2088[1].mOutputPulseIndicator.append(ui->po0_2);
    mNode2088[1].mOutputPulseIndicator.append(ui->po1_2);
    mNode2088[1].mOutputPulseIndicator.append(ui->po2_2);
    mNode2088[1].mOutputPulseIndicator.append(ui->po3_2);
    mNode2088[1].mOutputPulseIndicator.append(ui->po4_2);
    mNode2088[1].mOutputPulseIndicator.append(ui->po5_2);

    mNode2088[1].hiLimit.append(ui->hi0_2);
    mNode2088[1].hiLimit.append(ui->hi1_2);
    mNode2088[1].hiLimit.append(ui->hi2_2);
    mNode2088[1].hiLimit.append(ui->hi3_2);
    mNode2088[1].hiLimit.append(ui->hi4_2);
    mNode2088[1].hiLimit.append(ui->hi5_2);
    mNode2088[1].hiLimit.append(ui->hi6_2);
    mNode2088[1].hiLimit.append(ui->hi7_2);

    mNode2088[1].loLimit.append(ui->lo0_2);
    mNode2088[1].loLimit.append(ui->lo1_2);
    mNode2088[1].loLimit.append(ui->lo2_2);
    mNode2088[1].loLimit.append(ui->lo3_2);
    mNode2088[1].loLimit.append(ui->lo4_2);
    mNode2088[1].loLimit.append(ui->lo5_2);
    mNode2088[1].loLimit.append(ui->lo6_2);
    mNode2088[1].loLimit.append(ui->lo7_2);

    for (int i=0; i<PWM_COUNT; i++) {
       mNode2088[1].mOutputPulseIndicator[i]->setObjectName(QString::number(i));
       mNode2088[1].hiLimit[i]->setObjectName(QString::number(i));
       mNode2088[1].loLimit[i]->setObjectName(QString::number(i));
    }

    ///////////////////////////////////////////////////////////

    mNode2088[2].mCan = &mCan;
    mNode2088[2].mBox = ui->can2088_3;

    mNode2088[2].mInversion = ui->inversion_3;
    connect(mNode2088[2].mInversion, SIGNAL(toggled(bool)), &mNode2088[2], SLOT(setInversion(bool)));

    mNode2088[2].mButtons[0] = ui->po6_3;
    mNode2088[2].mButtons[1] = ui->po7_3;

    mNode2088[2].mFrequency = ui->frequencySlider_3;
    mNode2088[2].mFrequiencyIndicator = ui->frequencyIndicator_3;
    connect(mNode2088[2].mFrequency, SIGNAL(valueChanged(int)), &mNode2088[2], SLOT(setFrequency(int)));

    mNode2088[2].mOutputPulseIndicator.append(ui->po0_3);
    mNode2088[2].mOutputPulseIndicator.append(ui->po1_3);
    mNode2088[2].mOutputPulseIndicator.append(ui->po2_3);
    mNode2088[2].mOutputPulseIndicator.append(ui->po3_3);
    mNode2088[2].mOutputPulseIndicator.append(ui->po4_3);
    mNode2088[2].mOutputPulseIndicator.append(ui->po5_3);

    mNode2088[2].hiLimit.append(ui->hi0_3);
    mNode2088[2].hiLimit.append(ui->hi1_3);
    mNode2088[2].hiLimit.append(ui->hi2_3);
    mNode2088[2].hiLimit.append(ui->hi3_3);
    mNode2088[2].hiLimit.append(ui->hi4_3);
    mNode2088[2].hiLimit.append(ui->hi5_3);
    mNode2088[2].hiLimit.append(ui->hi6_3);
    mNode2088[2].hiLimit.append(ui->hi7_3);

    mNode2088[2].loLimit.append(ui->lo0_3);
    mNode2088[2].loLimit.append(ui->lo1_3);
    mNode2088[2].loLimit.append(ui->lo2_3);
    mNode2088[2].loLimit.append(ui->lo3_3);
    mNode2088[2].loLimit.append(ui->lo4_3);
    mNode2088[2].loLimit.append(ui->lo5_3);
    mNode2088[2].loLimit.append(ui->lo6_3);
    mNode2088[2].loLimit.append(ui->lo7_3);

    for (int i=0; i<PWM_COUNT; i++) {
       mNode2088[2].mOutputPulseIndicator[i]->setObjectName(QString::number(i));
       mNode2088[2].hiLimit[i]->setObjectName(QString::number(i));
       mNode2088[2].loLimit[i]->setObjectName(QString::number(i));
    }

    loadSettings();

    connect(&mPinger, SIGNAL(readyRead()), this, SLOT(slotReadPingAnser()));
    connect(&mPingTimer, SIGNAL(timeout()), this, SLOT(ping()));

    ///////////////////////////////////////////////////////////

    connect(&mServer, SIGNAL(newConnection()), this, SLOT(connection()));
    connect(&mCan, SIGNAL(initialized(int)), this, SLOT(canInitialized(int)));

    if (!mCan.init()) {
        QMessageBox::critical(0, "Startup Error!", "Can not load CAN driver!", QMessageBox::Ok);
    } else {
        if (!mServer.listen(QHostAddress::Any, SERVER_SOCKET)) {
            QMessageBox::critical(0, "Unable to start the server", mServer.errorString(), QMessageBox::Ok);
        } else {
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
    }

    move(0, 0);
}

Server::~Server()
{
    saveSettings();
    mServer.close();

    stop();

    delete ui;
}

void Server::loadSettings()
{
    QSettings settings("V-Channel", "Megatron-server");
    int size = settings.beginReadArray("Nodes");
    for (int i=0; i<size; i++) {
        for (int j=0; j<PWM_COUNT; j++) {
            connect(mNode2088[i].mOutputPulseIndicator[j], SIGNAL(valueChanged(int)), &mNode2088[i], SLOT(setDuty(int)));
        }

        settings.setArrayIndex(i);
        mNode2088[i].mInversion->setChecked(settings.value("inversion").toBool());
        int val = settings.value("frequency").toInt();
        qDebug() << "Set frequency (" << i << ") to " << val;
        mNode2088[i].mFrequency->setValue(val);
        settings.beginReadArray("Ports");
        for (int port=0; port<8; port++) {
            settings.setArrayIndex(port);
            mNode2088[i].hiLimit[port]->setValue(settings.value("hiLimit").toInt());
            mNode2088[i].loLimit[port]->setValue(settings.value("loLimit").toInt());
        }
        settings.endArray();
    }
    settings.endArray();


    for (int i=0; i<3; i++) {
        connect(mNode2088[i].mInversion, SIGNAL(toggled(bool)), this, SLOT(saveSettings()));
        connect(mNode2088[i].mFrequency, SIGNAL(sliderReleased()), this, SLOT(saveSettings()));
        for (int j=0; j<8; j++) {
           connect(mNode2088[i].hiLimit[j], SIGNAL(valueChanged(int)), this, SLOT(saveSettings()));
           connect(mNode2088[i].loLimit[j], SIGNAL(valueChanged(int)), this, SLOT(saveSettings()));
        }
    }

}

void Server::saveSettings()
{
    QSettings settings("V-Channel", "Megatron-server");
    settings.beginWriteArray("Nodes");
    for (int i=0; i<3; i++) {
        settings.setArrayIndex(i);
        settings.setValue("inversion", mNode2088[i].mInversion->isChecked());
        int val = mNode2088[i].mFrequency->value();
//        qDebug() << "Save frequency for (" << i << ") to " << val;
        settings.setValue("frequency", val);
        settings.beginWriteArray("Ports");
        for (int port=0; port<8; port++) {
            settings.setArrayIndex(port);
            settings.setValue("hiLimit", mNode2088[i].hiLimit[port]->value());
            settings.setValue("loLimit", mNode2088[i].loLimit[port]->value());
        }
        settings.endArray();
    }
    settings.endArray();
}

void Server::connection()
{
    start();

    QTcpSocket *client = mServer.nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    connect(client, SIGNAL(disconnected()), this, SLOT(slotDisconnectClient()));
}

void Server::onPingerConnected()
{
    mPingerConnected = true;
    mPingTimer.start(1000);
}

void Server::onPingerDisconnected()
{
    mPingerConnected = false;
}

void Server::onPingerDisplayError(QAbstractSocket::SocketError)
{
    QTcpSocket *server = reinterpret_cast<QTcpSocket*>(sender());
    if (server) {
        QMessageBox::critical(0, "Connection Error!", server->errorString(), QMessageBox::Ok);
        server->close();
        onPingerDisconnected();
    }
}

void Server::ping()
{
    if (mPingerConnected) {
        mPingerConnected = false;
        QVariantMap map;
        map.insert("CANType", CAN_2088);
        map.insert("CommandType", CAN_Acknowledge);
        QJsonObject answerCommand = QJsonObject::fromVariantMap(map);
        QByteArray answerData = QJsonDocument(answerCommand).toBinaryData();
        mPinger.write(answerData);

        qDebug() << "sent ping";
    } else {
        qDebug() << "ping error";
        mPingError++;
    }
    if (mPinger.isOpen()) {
        if (mPingError > 3) {
            mPingTimer.stop();
            stop();
        } else {
            mPingTimer.start(1000);
        }
    }
}

void Server::slotReadPingAnser()
{
    mPingError = 0;
    mPingerConnected = true;
//    qDebug() << "read ping answer";
}

void Server::shutdown()
{
    stop();
}

void Server::slotDisconnectClient()
{
    mPinger.close();
    mPingTimer.stop();
    mPingerConnected = false;
    stop();
}

void Server::slotReadClient()
{
    QTcpSocket *clientSocket = reinterpret_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        QByteArray data = clientSocket->readAll();
        if ((char)(data[0]) == 1 && (char)(data[1]) == 9 && (char)(data[2]) == 6 && (char)(data[3]) == 4) {
            shutdown();
        } else {
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
                    QJsonValue node = command.take("Node");
                    QJsonValue port = command.take("Port");
                    QJsonValue value = command.take("Value");
                    set2057Value(node.toInt(), port.toInt(), value.toBool());
                } else if (canType.toInt() == CAN_2088) {
                    QJsonArray values = command.take("PortArray").toArray();
                    for (int i=0; i<values.count(); i++) {
                        QJsonObject item = values[i].toObject();
                        QJsonValue node = item.take("Node");
                        QJsonValue port = item.take("Port");
                        QJsonValue value = item.take("Value");
    //                    qDebug() << "node " << node.toInt() << " port " << port.toInt() << " value " << value.toInt();
                        mNode2088[node.toInt()-1].setValue(port.toInt(), value.toInt());
                    }
                }
            } else if (commandType.toInt() == CAN_Alive) {
                connect(&mPinger, SIGNAL(connected()), this, SLOT(onPingerConnected()));
                connect(&mPinger, SIGNAL(disconnected()), this, SLOT(onPingerDisconnected()));
                connect(&mPinger, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onPingerDisplayError(QAbstractSocket::SocketError)));
                mPinger.connectToHost(clientSocket->peerAddress(), PING_SOCKET);
            }
        }
    }
}

void Server::canInitialized(int node)
{
    int canType = mCan.getCanType(node);
    if (canType == 0x2088) {
        mNode2088[node-1].mBox->setEnabled(true);
        mNode2088[node-1].mBox->setTitle("2088 ["+QString::number(node)+"]");
        mNode2088[node-1].init(node);
    } else if (canType == 0x2057) {
        int index = node == 5 ? 1 : 0;
        mNode2057[index] = node;
        if (index == 0) {
            ui->can2057_1->setEnabled(true);
            ui->can2057_1->setTitle("2057 ["+QString::number(node)+"]");
        } else {
            ui->can2057_2->setEnabled(true);
        }
        for (int i=0; i<16; i++) {
            connect(mOutputIndicator[index][i], SIGNAL(stateChanged(int)), this, SLOT(set2057port(int)));
        }
    }
}

void Server::set2057port(int state)
{
    int port = sender()->objectName().toInt();
    int index = 0;
    if (port >= 16) {
        port -= 16;
        index = 1;
    }
    UNS8 value;
    if (port < 8) {
        m2057lowValue[index].set(port, state > 0 ? 1 : 0);
        value = m2057lowValue[index].to_ulong();
    } else {
        m2057highValue[index].set(port-8, state > 0 ? 1 : 0);
        value = m2057highValue[index].to_ulong();
    }
    if (mNode2057[index] > 0) {
        mCan.setTrigger(mNode2057[index], port, value);
    }
}

void Server::set2057Value(int node, int port, bool isOn)
{
    int index = node == 5 ? 1 : 0;
    mOutputIndicator[index][port]->setChecked(isOn);
}

void Server::start() {
    for (int i=0; i<3; i++) {
        mNode2088[i].start();
    }
    mPingError = 0;
    Alive = true;
}

void Server::stop() {
    for (int index=0; index<2; index++) {
        for (int i=0; i<16; i++) {
            mOutputIndicator[index][i]->setChecked(false);
        }
    }
    for (int i=0; i<3; i++) {
        mNode2088[i].stop();
    }
    Alive = false;
    mPingError = 0;
}
