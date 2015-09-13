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

    mLeftNode[0] = 3; mRightNode[0] = 0;
    mLeftNode[1] = 1; mRightNode[1] = 2;
    mLeftNode[2] = 0; mRightNode[2] = 0;
    mLeftNode[3] = 0; mRightNode[3] = 0;

    mCurrentMode = 0;
    mModes[0] = ui->mode_1;
    mModes[1] = ui->mode_2;
    mModes[2] = ui->mode_3;
    mModes[3] = ui->mode_4;
    for (int i=0; i<NUM_MODE; i++) {
        mModes[i]->setObjectName(QString::number(i));
        connect(mModes[i], SIGNAL(toggled(bool)), this, SLOT(switchMode(bool)));
    }

    loadSettings();

    connect(ui->clearHistory, SIGNAL(clicked()), this, SLOT(clearHistory()));
    connect(ui->addControl, SIGNAL(clicked()), this, SLOT(addControl()));
    connect(ui->clearControls, SIGNAL(clicked()), this, SLOT(clearControls()));
    
    ui->connectButton->setStyleSheet("background-color:green; color: white;");
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(start(bool)));
    
    // left joystick panel

    connect(ui->inversion_1, SIGNAL(toggled(bool)), this, SLOT(setInversion(bool)));
    connect(ui->inversion_2, SIGNAL(toggled(bool)), this, SLOT(setInversion(bool)));

    ui->joystick_1->connectControls(ui->frequency_1, ui->frequencyIndicator_1, ui->lowLimit_1, ui->lowLimitIndicator_1,
                                  ui->highLimit_1, ui->highLimitIndicator_1, ui->joystickMonitor_1);
    connect(ui->joystick_1, SIGNAL(sendFrequency(int)), this, SLOT(setFrequency(int)));
    connect(ui->joystick_1, SIGNAL(sendLowLimit(int)), this, SLOT(setLowLevel(int)));
    connect(ui->joystick_1, SIGNAL(sendHighLimit(int)), this, SLOT(setHighLevel(int)));
    connect(ui->joystickMonitor_1, SIGNAL(setLevel(const QVector<int>&)), this, SLOT(setLevel(const QVector<int>&)));
    ui->frequency_1->setValue(mLeftFrequency[0]);
    ui->inversion_1->setChecked(mLeftInversion[0]);
    ui->highLimit_1->setValue(mLeftTopLevel[0]);
    ui->lowLimit_1->setValue(mLeftLowLevel[0]);

    // right joystick panel

    ui->joystick_2->connectControls(ui->frequency_2, ui->frequencyIndicator_2, ui->lowLimit_2, ui->lowLimitIndicator_2,
                                    ui->highLimit_2, ui->highLimitIndicator_2, ui->joystickMonitor_2);
    connect(ui->joystick_2, SIGNAL(sendFrequency(int)), this, SLOT(setFrequency(int)));
    connect(ui->joystick_2, SIGNAL(sendLowLimit(int)), this, SLOT(setLowLevel(int)));
    connect(ui->joystick_2, SIGNAL(sendHighLimit(int)), this, SLOT(setHighLevel(int)));
    connect(ui->joystickMonitor_2, SIGNAL(setLevel(const QVector<int>&)), this, SLOT(setLevel(const QVector<int>&)));
    ui->frequency_2->setValue(mRightFrequency[0]);
    ui->inversion_2->setChecked(mRightInversion[0]);
    ui->highLimit_2->setValue(mRightTopLevel[0]);
    ui->lowLimit_2->setValue(mRightLowLevel[0]);

    // server

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
    if (mJoystick.size() > 0 ) {
        ui->joystick_1->setEnabled(true);
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

void Client::switchMode(bool isOn)
{
    if (isOn) {
        mCurrentMode = sender()->objectName().toInt();
        for (int i=0; i<mControlButtons[mCurrentMode].size(); i++) {
            QWidget *w = mControlButtons[mCurrentMode].at(i);
            w->setHidden(false);
            ui->controlLayout->addWidget(w);
        }
        ui->frequency_1->setValue(mLeftFrequency[mCurrentMode]);
        ui->frequency_2->setValue(mRightFrequency[mCurrentMode]);
        ui->inversion_1->setChecked(mLeftInversion[mCurrentMode]);
        ui->inversion_2->setChecked(mRightInversion[mCurrentMode]);
        ui->highLimit_1->setValue(mLeftTopLevel[mCurrentMode]);
        ui->highLimit_2->setValue(mRightTopLevel[mCurrentMode]);
        ui->lowLimit_1->setValue(mLeftLowLevel[mCurrentMode]);
        ui->lowLimit_2->setValue(mRightLowLevel[mCurrentMode]);

        if (mJoystick.size() > 0 && mLeftNode[mCurrentMode] > 0) {
            ui->joystick_1->setEnabled(true);
        } else {
            ui->joystick_1->setEnabled(false);
        }
        if (mJoystick.size() > 1 && mRightNode[mCurrentMode] > 0) {
            ui->joystick_2->setEnabled(true);
        } else {
            ui->joystick_2->setEnabled(false);
        }

    } else {
        for (int i=0; i<mControlButtons[mCurrentMode].size(); i++) {
            QWidget *w = mControlButtons[mCurrentMode].at(i);
            w->setHidden(true);
            ui->controlLayout->removeWidget(w);
        }
    }
    ui->controlLayout->update();
}

void Client::clearControls()
{
    for (int i=0; i<mControlButtons[mCurrentMode].size(); i++) {
        QWidget *w = mControlButtons[mCurrentMode].at(i);
        ui->controlLayout->removeWidget(w);
        delete w;
    }
    mControlButtons[mCurrentMode].clear();
    saveSettings();
}

void Client::addControl()
{
    ButtonSetup *setup = new ButtonSetup();
    if (setup->exec() == QDialog::Accepted) {
        ControlButton* control = new ControlButton(this);
        connect(control, SIGNAL(setLevel(int,bool)), this, SLOT(setLevel(int,bool)));
        setup->setupButton(control);
        control->setMinimumHeight(40);
        mControlButtons[mCurrentMode].append(control);
        ui->controlLayout->addWidget(control);
    }
    saveSettings();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

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

    size = settings.beginReadArray("modes");
    for (int m=0; m<NUM_MODE; m++) {
        settings.setArrayIndex(m);
        mLeftFrequency[m] = settings.value("leftFreq").toInt();
        mRightFrequency[m] = settings.value("rightFreq").toInt();
        mLeftInversion[m] = settings.value("leftInvert").toBool();
        mRightInversion[m] = settings.value("rightInvert").toBool();

        mLeftTopLevel[m] = settings.value("leftTopLevel").toInt();
        mLeftLowLevel[m] = settings.value("leftLowLevel").toInt();
        mRightTopLevel[m] = settings.value("rightTopLevel").toInt();
        mRightLowLevel[m] = settings.value("rightLowLevel").toInt();

        int num_buttons = settings.value("buttons_count").toInt();
        if (num_buttons > 0) {
            size = settings.beginReadArray("buttons");
            for (int i = 0; i < size; ++i) {
                settings.setArrayIndex(i);
                ButtonConfig config;
                config.name = settings.value("name").toString();
                config.checkable = settings.value("checkable").toBool();
                config.inverse = settings.value("inverse").toBool();
                config.port = settings.value("port").toInt();

                ControlButton* control = new ControlButton(this);
                connect(control, SIGNAL(setLevel(int,bool)), this, SLOT(setLevel(int,bool)));
                control->setConfig(config);
                control->setMinimumHeight(40);
                mControlButtons[m].append(control);
            }
            settings.endArray();
        }
    }
    settings.endArray();

    for (int i=0; i<mControlButtons[0].size(); i++) {
        QWidget *w = mControlButtons[0].at(i);
        w->setHidden(false);
        ui->controlLayout->addWidget(w);
    }
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

    settings.beginWriteArray("modes");
    for (int m=0; m<NUM_MODE; m++) {
        settings.setArrayIndex(m);
        settings.setValue("leftFreq", mLeftFrequency[m]);
        settings.setValue("rightFreq", mRightFrequency[m]);
        settings.setValue("leftInvert", mLeftInversion[m]);
        settings.setValue("rightInvert", mRightInversion[m]);

        settings.setValue("leftTopLevel", mLeftTopLevel[m]);
        settings.setValue("leftLowLevel", mLeftLowLevel[m]);
        settings.setValue("rightTopLevel", mRightTopLevel[m]);
        settings.setValue("rightLowLevel", mRightLowLevel[m]);

        settings.setValue("buttons_count", mControlButtons[m].size());
        if (mControlButtons[m].size() > 0) {
            settings.beginWriteArray("buttons");
            for (int i=0; i<mControlButtons[m].size(); i++) {
                settings.setArrayIndex(i);
                ControlButton* button = mControlButtons[m].at(i);
                ButtonConfig config = button->config();
                settings.setValue("name", config.name);
                settings.setValue("checkable", config.checkable);
                settings.setValue("inverse", config.inverse);
                settings.setValue("port", config.port);
            }
            settings.endArray();
        }
    }
    settings.endArray();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Client::setInversion(bool isOn)
{
    if (sender() == ui->inversion_1) {
        mLeftInversion[mCurrentMode] = isOn;
    } else if (sender() == ui->inversion_1) {
        mRightInversion[mCurrentMode] = isOn;
    }
    saveSettings();
}

void Client::setLowLevel(int level)
{
    if (sender() == ui->joystick_1) {
        mLeftLowLevel[mCurrentMode] = level;
    } else if (sender() == ui->joystick_2) {
        mRightLowLevel[mCurrentMode] = level;
    }
    saveSettings();
}

void Client::setHighLevel(int level)
{
    if (sender() == ui->joystick_1) {
        mLeftTopLevel[mCurrentMode] = level;
    } else if (sender() == ui->joystick_2) {
        mRightTopLevel[mCurrentMode] = level;
    }
    saveSettings();
}

void Client::setFrequency(int frequency)
{
    if (sender() == ui->joystick_1) {
        mLeftFrequency[mCurrentMode] = frequency;
    } else if (sender() == ui->joystick_2) {
        mRightFrequency[mCurrentMode] = frequency;
    }
    saveSettings();
    
    if (mServer.isOpen()) {
        QVariantMap map;
        map.insert("CANType", CAN_2088);
        map.insert("CommandType", CAN_SetPreference);
        map.insert("Node", 3);
        map.insert("Value", frequency);
        
        QJsonObject command = QJsonObject::fromVariantMap(map);
        QByteArray data = QJsonDocument(command).toBinaryData();
        mServer.write(data);
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
    int node = 0;
    if (sender() == ui->joystick_1) {
        node = mLeftNode[mCurrentMode];
    } else if (sender() == ui->joystick_2) {
        node = mRightNode[mCurrentMode];
    }

    if (mServer.isOpen()) {
        QVariantMap map;
        map.insert("CANType", CAN_2088);
        map.insert("CommandType", CAN_SetValue);

        QVariantList list;
        for (int i=0; i<values.count(); i++) {
            QVariantMap p0;
            p0.insert("Node", node);
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

////////////////////////////////////////////////////////////////////////////////////////////////////

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
