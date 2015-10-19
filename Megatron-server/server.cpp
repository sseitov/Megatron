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

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server), mClient(0), mNode2057(0)
{
    ui->setupUi(this);

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

    mNode2088[0].loLimit.append(ui->lo0_1);
    mNode2088[0].loLimit.append(ui->lo1_1);
    mNode2088[0].loLimit.append(ui->lo2_1);
    mNode2088[0].loLimit.append(ui->lo3_1);
    mNode2088[0].loLimit.append(ui->lo4_1);
    mNode2088[0].loLimit.append(ui->lo5_1);

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

    mNode2088[1].loLimit.append(ui->lo0_2);
    mNode2088[1].loLimit.append(ui->lo1_2);
    mNode2088[1].loLimit.append(ui->lo2_2);
    mNode2088[1].loLimit.append(ui->lo3_2);
    mNode2088[1].loLimit.append(ui->lo4_2);
    mNode2088[1].loLimit.append(ui->lo5_2);

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

    mNode2088[2].loLimit.append(ui->lo0_3);
    mNode2088[2].loLimit.append(ui->lo1_3);
    mNode2088[2].loLimit.append(ui->lo2_3);
    mNode2088[2].loLimit.append(ui->lo3_3);
    mNode2088[2].loLimit.append(ui->lo4_3);
    mNode2088[2].loLimit.append(ui->lo5_3);

    for (int i=0; i<PWM_COUNT; i++) {
       mNode2088[2].mOutputPulseIndicator[i]->setObjectName(QString::number(i));
       mNode2088[2].hiLimit[i]->setObjectName(QString::number(i));
       mNode2088[2].loLimit[i]->setObjectName(QString::number(i));
    }

    loadSettings();

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
    if (mClient) {
        mClient->close();
        mClient = 0;
    }
    mServer.close();
    reset2057();
    for (int i=0; i<3; i++) {
        mNode2088[i].reset();
    }
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
        for (int port=0; port<PWM_COUNT; port++) {
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
        for (int j=0; j<PWM_COUNT; j++) {
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
        for (int port=0; port<PWM_COUNT; port++) {
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
    if (mClient) {
        mClient->close();
    }
    mClient = mServer.nextPendingConnection();
    connect(mClient, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    connect(mClient, SIGNAL(disconnected()), this, SLOT(slotDisconnectClient()));
    for (int i=0; i<3; i++) {
        if (mNode2088[i].mNode >= 0) {
            mNode2088[i].set();
        }
    }
}

void Server::slotDisconnectClient()
{
    mClient = 0;
    reset2057();
    for (int i=0; i<3; i++) {
        mNode2088[i].reset();
    }
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
                    QJsonObject item = values[i].toObject();
                    QJsonValue node = item.take("Node");
                    QJsonValue port = item.take("Port");
                    QJsonValue value = item.take("Value");
//                    qDebug() << "node " << node.toInt() << " port " << port.toInt() << " value " << value.toInt();
                    mNode2088[node.toInt()-1].setValue(port.toInt(), value.toInt());
                }
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
        mNode2088[node-1].start(node);
    } else if (canType == 0x2057) {
        mNode2057 = node;
        ui->can2057->setEnabled(true);
        ui->can2057->setTitle("2057 ["+QString::number(node)+"]");
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
    if (mNode2057 > 0) {
        mCan.setTrigger(mNode2057, port, value);
    }
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
