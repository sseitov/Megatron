#include "client.h"
#include "ui_client.h"
#include "buttonsetup.h"
#include "../common.h"
#include "joystickmonitor.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QSettings>

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);


    loadSettings();
    
    connect(ui->clearHistory, SIGNAL(clicked()), this, SLOT(clearHistory()));
    
    ui->connectButton->setStyleSheet("background-color:green; color: white;");
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(start(bool)));
    
    ui->joystick_1->connectControls(ui->frequency_1, ui->frequencyIndicator_1, ui->lowLimit_1, ui->lowLimitIndicator_1,
                                  ui->highLimit_1, ui->highLimitIndicator_1, ui->joystickMonitor_1);
    connect(ui->joystickMonitor_1, SIGNAL(setLevel(const QVector<int>&)), this, SLOT(setLevel(const QVector<int>&)));

    ui->joystick_2->connectControls(ui->frequency_2, ui->frequencyIndicator_2, ui->lowLimit_2, ui->lowLimitIndicator_2,
                                    ui->highLimit_2, ui->highLimitIndicator_2, ui->joystickMonitor_2);
    connect(ui->joystickMonitor_2, SIGNAL(setLevel(const QVector<int>&)), this, SLOT(setLevel(const QVector<int>&)));
    
/*
    for (int i=0; i<16; i++) {
        mInputButton[i]->setObjectName(QString::number(i));
        connect(mInputButton[i], SIGNAL(toggled(bool)), this, SLOT(connectInput(bool)));
        connect(mInputButton[i], SIGNAL(setLevel(int,bool)), this, SLOT(setLevel(int,bool)));
    }
*/
    connect(&mServer, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(&mServer, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(&mServer, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(&mServer, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    for (int i=0; i<SDL_NumJoysticks(); i++) {
        QJoystick *joy = new QJoystick(i, SDL_JoystickOpen(i));
        connect(joy, SIGNAL(setData(int, qreal, qreal, bool, bool)), this, SLOT(setJoystickData(int, qreal, qreal, bool, bool)));
        mJoystick.append(joy);
    }
    if (mJoystick.size() < 2) {
        ui->joystick_2->setEnabled(false);
    }
    if (mJoystick.size() < 1) {
        ui->joystick_1->setEnabled(false);
    }
}

Client::~Client()
{
    for (int i=0; i<mJoystick.size(); i++) {
        QJoystick* joy = mJoystick[i];
        delete joy;
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    saveSettings();
    delete ui;
}

void Client::setJoystickData(int num, qreal x, qreal y, bool b1, bool b2)
{
    x = x/32765.0*JOYSTICK_RADIUS;
    y = y/32765.0*JOYSTICK_RADIUS;
    if (num == 0) {
        ui->joystickMonitor_1->setTarget(x, y);
        ui->button1_1->setChecked(b1);
        ui->button2_1->setChecked(b2);
    } else {
        ui->joystickMonitor_2->setTarget(x, y);
        ui->button1_2->setChecked(b1);
        ui->button2_2->setChecked(b2);
    }
}

void Client::clearHistory()
{
    ui->ipAddress->clear();
    saveSettings();
}

void Client::loadSettings()
{
    QSettings settings("V-Channel", "Megatron-client");
    
    int size = settings.beginReadArray("history");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString ip = settings.value("ip").toString();
        ui->ipAddress->addItem(ip);
    }
    settings.endArray();
 /*
    size = settings.beginReadArray("buttons");
    if (size > 0) {
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);
            ButtonConfig config;
            config.name = settings.value("name").toString();
            config.checkable = settings.value("checkable").toBool();
            config.inverse = settings.value("inverse").toBool();
            config.port = settings.value("port").toInt();
            mInputButton[i]->setConfig(config);
        }
    } else {
        for (int i = 0; i < 16; ++i) {
            ButtonConfig config;
            mInputButton[i]->setConfig(config);
        }
    }
    settings.endArray();*/
}

void Client::saveSettings()
{
    QSettings settings("V-Channel", "Megatron-client");
    
    settings.beginWriteArray("history");
    for (int i = 0; i < ui->ipAddress->count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("ip", ui->ipAddress->itemText(i));
    }
    settings.endArray();
/*
    settings.beginWriteArray("buttons");
    for (int i = 0; i < 16; ++i) {
        settings.setArrayIndex(i);
        ButtonConfig config = mInputButton[i]->config();
        settings.setValue("name", config.name);
        settings.setValue("checkable", config.checkable);
        settings.setValue("inverse", config.inverse);
        settings.setValue("port", config.port);
    }
    settings.endArray();*/
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
            saveSettings();
        } else {
            control->resetConfig();
        }
    }
}

void Client::setLevel(int port, bool value)
{
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

void Client::setLevel(const QVector<int>& values)
{
    if (mServer.isOpen()) {
        QVariantMap map;
        map.insert("CANType", CAN_2088);
        map.insert("CommandType", CAN_SetValue);

        QVariantList list;
        for (int i=0; i<values.count(); i++) {
            QVariantMap p0;
            p0.insert("Node", 3);
            p0.insert("Port", i);
            p0.insert("Value", values[i]);
            list.append(p0);
        }

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
        mServer.connectToHost(ui->ipAddress->currentText(), SERVER_SOCKET);
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
            QJsonValue canArray = command.take("CANArray");
            if (canArray == QJsonValue::Undefined)
                return;
            QJsonArray cans = canArray.toArray();
            for (int i=0; i<cans.count(); i++) {
                QJsonObject can = cans[i].toObject();
                
                QJsonValue type = can.take("CANType");
                QJsonValue node = can.take("Node");
/*
                if (type.toInt() == CAN_2057) {
                    ui->can_2057->setEnabled(true);
                }
                if (type.toInt() == CAN_2088) {
                    ui->joystick->setEnabled(true);
                    ui->joystickMonitor->setTarget(0, 0);
                }
 */
            }
        }
    }
}

void Client::onSokConnected()
{
    if (ui->ipAddress->findText(ui->ipAddress->currentText()) < 0) {
        ui->ipAddress->insertItem(0, ui->ipAddress->currentText());
        saveSettings();
    }
    ui->connectButton->setText(tr("Отсоединить"));
    ui->connectButton->setStyleSheet("background-color:red; color: white;");
}

void Client::onSokDisconnected()
{
    ui->connectButton->setChecked(false);
    ui->connectButton->setText(tr("Соединить"));
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
