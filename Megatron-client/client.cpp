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

#define NODE_2057   4

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    ui->resetButton->setStyleSheet("background-color:grey; color: white;");
    ui->resetButton->setEnabled(false);
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(shutdown()));
    
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
    connect(ui->joystickMonitor_1, SIGNAL(setLevel(const QVector<int>&)), this, SLOT(setLevel(const QVector<int>&)));

    // right joystick panel
    connect(ui->joystickMonitor_2, SIGNAL(setLevel(const QVector<int>&)), this, SLOT(setLevel(const QVector<int>&)));

    // server
    connect(&mPingServer, SIGNAL(newConnection()), this, SLOT(pingConnection()));

    connect(&mServer, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(&mServer, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(&mServer, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    for (int i=0; i<SDL_NumJoysticks(); i++) {
        QJoystick *joy = new QJoystick(i, SDL_JoystickOpen(i));
        connect(joy, SIGNAL(setAxiz(int, int, int, int)), this, SLOT(setJoystickAxiz(int, int, int, int)));
        connect(joy, SIGNAL(setButtons(int, bool, bool)), this, SLOT(setJoystickButtons(int, bool, bool)));
        mJoystick.append(joy);
    }

    connect(ui->button1_1, SIGNAL(toggled(bool)), this, SLOT(setButton(bool)));
    ui->button1_1->setObjectName(QString::number(1));
    connect(ui->button2_1, SIGNAL(toggled(bool)), this, SLOT(setButton(bool)));
    ui->button2_1->setObjectName(QString::number(2));

    connect(ui->button1_2, SIGNAL(toggled(bool)), this, SLOT(setButton(bool)));
    ui->button1_2->setObjectName(QString::number(3));
    connect(ui->button2_2, SIGNAL(toggled(bool)), this, SLOT(setButton(bool)));
    ui->button2_2->setObjectName(QString::number(4));
    
    if (mJoystick.size() > 0 ) {
        ui->joystick_1->setEnabled(true);
    }
    move(0, 0);
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

void Client::setJoystickAxiz(int num, int x, int y, int z)
{
    qDebug() << "AXIZ " << x << " :: " << y;
    qreal xAxiz = (qreal)x/100.0*JOYSTICK_RADIUS;
    qreal yAxiz = (qreal)y/100.0*JOYSTICK_RADIUS;
    qreal zAxiz = (qreal)z/100.0*JOYSTICK_RADIUS;
    if (num == 0) {
        ui->joystickMonitor_1->setTarget(xAxiz, yAxiz, zAxiz);
    } else {
        ui->joystickMonitor_2->setTarget(xAxiz, yAxiz, zAxiz);
    }
}

void Client::setJoystickButtons(int num, bool b1, bool b2)
{
    if (num == 0) {
        ui->button1_1->setChecked(b1);
        ui->button2_1->setChecked(b2);
    } else {
        ui->button1_2->setChecked(b1);
        ui->button2_2->setChecked(b2);
    }
}

void Client::setButton(bool checked)
{
    int node = 0;
    int port = 0;
    int num = sender()->objectName().toInt();

    switch (num) {
        case 1:
            node = mLeftNode[mCurrentMode];
            port = 6;
            break;
        case 2:
            node = mLeftNode[mCurrentMode];
            port = 7;
            break;
        case 3:
            node = mRightNode[mCurrentMode];
            port = 6;
            break;
        case 4:
            node = mRightNode[mCurrentMode];
            port = 7;
            break;
        default:
            return;
    }
    
    
    if (mServer.isOpen() && node > 0) {
        QVariantMap map;
        map.insert("CANType", CAN_2088);
        map.insert("CommandType", CAN_SetValue);
        
        QVariantList list;
        
        if (mCurrentMode == 1) {
            QVariantMap p0;
            p0.insert("Node", node);
            p0.insert("Port", port);
            int value = checked ? 999 : 1;
            p0.insert("Value", value);
            list.append(p0);
            
            QVariantMap p1;
            p1.insert("Node", 3);
            p1.insert("Port", 4);
            p1.insert("Value", checked ? 1 : 999);
            list.append(p1);
        } else  {
            QVariantMap p0;
            p0.insert("Node", 1);
            p0.insert("Port", port);
            p0.insert("Value", checked ? 999 : 1);
            list.append(p0);
            
            QVariantMap p1;
            p1.insert("Node", 3);
            p1.insert("Port", 4);
            p1.insert("Value", checked ? 1 : 999);
            list.append(p1);
        }
        map.insert("PortArray", list);
        
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
        map.insert("Node", NODE_2057);
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
    if (sender() == ui->joystickMonitor_1) {
        node = mLeftNode[mCurrentMode];
    } else if (sender() == ui->joystickMonitor_2) {
        node = mRightNode[mCurrentMode];
    }


    if (mServer.isOpen() && node > 0) {
        writeValuesToNode(values, node);
    }
}

void Client::writeValuesToNode(const QVector<int>& values, int node)
{
    QVariantMap map;
    map.insert("CANType", CAN_2088);
    map.insert("CommandType", CAN_SetValue);

    int additionalPort = 800;
    QVariantList list;
    for (int i=0; i<values.count(); i++) {
        if (mCurrentMode == 0 && (i == 4 || i == 5))
            continue;
        QVariantMap p0;
        p0.insert("Node", node);
        p0.insert("Port", i);
        p0.insert("Value", values[i]);
        if (values[i] > 0)
            additionalPort = 1;
        list.append(p0);
    }
    if (node < 3) {
        QVariantMap p1;
        p1.insert("Node", 3);
        p1.insert("Port", 4);
        p1.insert("Value", additionalPort);
        list.append(p1);
    }
    map.insert("PortArray", list);

    QJsonObject command = QJsonObject::fromVariantMap(map);
    QByteArray data = QJsonDocument(command).toBinaryData();
    mServer.write(data);
}

void Client::shutdown()
{
    QByteArray data;
    data.append(1);
    data.append(9);
    data.append(6);
    data.append(4);
    mServer.write(data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Client::start(bool start)
{
    if (start) {
        if (mServer.isOpen()) {
            mServer.close();
        }
        
        if (!mPingServer.listen(QHostAddress::Any, PING_SOCKET)) {
            QMessageBox::critical(0, "Unable to start the ping server", mPingServer.errorString(), QMessageBox::Ok);
        }
        mServer.connectToHost(ui->ipAddress->currentText(), SERVER_SOCKET);
    } else {
        mPingServer.close();
        if (mServer.isOpen()) {
            mServer.close();
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
    ui->resetButton->setStyleSheet("background-color:black; color: white;");
    ui->resetButton->setEnabled(true);
    
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QVariantMap map;
    map.insert("CANType", CAN_2088);
    map.insert("CommandType", CAN_Alive);
    QJsonObject answerCommand = QJsonObject::fromVariantMap(map);
    QByteArray answerData = QJsonDocument(answerCommand).toBinaryData();
    socket->write(answerData);
}

void Client::pingConnection()
{
    QTcpSocket *client = mPingServer.nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(slotReadPing()));
    connect(client, SIGNAL(disconnected()), this, SLOT(slotDisconnectPing()));
}

void Client::slotReadPing()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    if (socket) {
        QByteArray data = socket->readAll();
        QJsonDocument request = QJsonDocument::fromBinaryData(data);
        QJsonObject command = request.object();
        QJsonValue commandType = command.take("CommandType");
        if (commandType == QJsonValue::Undefined)
            return;
        if (commandType.toInt() == CAN_Acknowledge) {
            QVariantMap map;
            map.insert("CANType", CAN_2088);
            map.insert("CommandType", CAN_Alive);
            QJsonObject answerCommand = QJsonObject::fromVariantMap(map);
            QByteArray answerData = QJsonDocument(answerCommand).toBinaryData();
            socket->write(answerData);
        }
    }
}

void Client::slotDisconnectPing()
{
}

void Client::onSokDisconnected()
{
    ui->connectButton->setChecked(false);
    ui->connectButton->setText(tr("Соединить"));
    ui->connectButton->setStyleSheet("background-color:green; color: white;");
    ui->resetButton->setStyleSheet("background-color:grey; color: white;");
    ui->resetButton->setEnabled(false);
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
