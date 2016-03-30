#include "videoclient.h"
#include "ui_videoclient.h"
#include "buttonsetup.h"
#include "../common.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QSettings>

VideoClient::VideoClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoClient)
{
    ui->setupUi(this);
    
    loadSettings();
    
    connect(ui->clearHistory, SIGNAL(clicked()), this, SLOT(clearHistory()));
    connect(ui->clearVideoHistory, SIGNAL(clicked()), this, SLOT(clearVideoHistory()));
    connect(ui->addControl, SIGNAL(clicked()), this, SLOT(addControl()));
    connect(ui->clearControls, SIGNAL(clicked()), this, SLOT(clearControls()));
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(start(bool)));

    connect(ui->videoConnectButton, SIGNAL(clicked()), this, SLOT(videoStart()));
    
    // server
    
    connect(&mServer, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(&mServer, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(&mServer, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));

    move(0, 0);
}

VideoClient::~VideoClient()
{
    saveSettings();
    delete ui;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void VideoClient::loadSettings()
{
    QSettings settings("V-Channel", "Megatron-video2");
    
    int size = settings.beginReadArray("history");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString ip = settings.value("ip").toString();
        ui->ipAddress->addItem(ip);
    }
    settings.endArray();
    
    size = settings.beginReadArray("video_history");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString ip = settings.value("url").toString();
        ui->videoURL->addItem(ip);
    }
    settings.endArray();
    
    int num_buttons = settings.value("buttons_count").toInt();
    if (num_buttons > 0) {
        size = settings.beginReadArray("buttons");
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);
            ButtonConfig config;
            config.name = settings.value("name").toString();
            config.checkable = settings.value("checkable").toBool();
            config.inverse = settings.value("inverse").toBool();
            config.canID = settings.value("canID").toInt();
            config.port = settings.value("port").toInt();
            
            ControlButton* control = new ControlButton(this);
            connect(control, SIGNAL(setLevel(int, int, bool)), this, SLOT(setLevel(int, int, bool)));
            control->setConfig(config);
            control->setMinimumHeight(40);
            mControlButtons.append(control);
        }
        settings.endArray();
    }
    
    for (int i=0; i<mControlButtons.size(); i++) {
        QWidget *w = mControlButtons.at(i);
        w->setHidden(false);
        int row = i % 4;
        int column = i / 4;
        ui->controlLayout->addWidget(w, row, column, Qt::AlignTop | Qt::AlignCenter);
    }
}

void VideoClient::saveSettings()
{
    QSettings settings("V-Channel", "Megatron-video2");
    
    settings.beginWriteArray("history");
    for (int i = 0; i < ui->ipAddress->count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("ip", ui->ipAddress->itemText(i));
    }
    settings.endArray();
    
    settings.beginWriteArray("video_history");
    for (int i = 0; i < ui->videoURL->count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("url", ui->videoURL->itemText(i));
    }
    settings.endArray();
    
    settings.setValue("buttons_count", mControlButtons.size());
    if (mControlButtons.size() > 0) {
        settings.beginWriteArray("buttons");
        for (int i=0; i<mControlButtons.size(); i++) {
            settings.setArrayIndex(i);
            ControlButton* button = mControlButtons.at(i);
            ButtonConfig config = button->config();
            settings.setValue("name", config.name);
            settings.setValue("checkable", config.checkable);
            settings.setValue("inverse", config.inverse);
            settings.setValue("canID", config.canID);
            settings.setValue("port", config.port);
        }
        settings.endArray();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VideoClient::clearHistory()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Очистить историю соединений с управляющим сервером?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }
    ui->ipAddress->clear();
    saveSettings();
}

void VideoClient::clearVideoHistory()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Очистить историю соединений с видео сервером?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }
    ui->videoURL->clear();
    saveSettings();
}

void VideoClient::clearControls()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Удалить все созданные элементы управления?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }
    for (int i=0; i<mControlButtons.size(); i++) {
        QWidget *w = mControlButtons.at(i);
        ui->controlLayout->removeWidget(w);
        delete w;
    }
    mControlButtons.clear();
    saveSettings();
}

void VideoClient::addControl()
{
    ButtonSetup *setup = new ButtonSetup();
    if (setup->exec() == QDialog::Accepted) {
        ControlButton* control = new ControlButton(this);
        connect(control, SIGNAL(setLevel(int, int,bool)), this, SLOT(setLevel(int, int,bool)));
        setup->setupButton(control);
        control->setMinimumHeight(40);
        mControlButtons.append(control);
        int row = (mControlButtons.size()-1) % 4;
        int column = (mControlButtons.size()-1) / 4;

        ui->controlLayout->addWidget(control, row, column, Qt::AlignTop | Qt::AlignCenter);
    }
    saveSettings();
}

void VideoClient::setLevel(int canID, int port, bool value)
{
    ControlButton *btn = (ControlButton*)sender();
    if (value) {
        btn->setChecked(true);
        for (int i=0; i < mControlButtons.size(); i++) {
            ControlButton *b = mControlButtons[i];
            if (b != btn && b->isChecked()) {
                if (b->isChecked()) {
                    b->sendLevel(false);
                    setLevel(canID, port, false);
                }
            }
        }
    }

    if (mServer.isOpen()) {
        QVariantMap map;
        map.insert("CANType", CAN_2057);
        map.insert("CommandType", CAN_SetValue);
        map.insert("Node", canID);
        map.insert("Port", port);
        map.insert("Value", value);
        QJsonObject command = QJsonObject::fromVariantMap(map);
        QByteArray data = QJsonDocument(command).toBinaryData();
        mServer.write(data);
        qDebug() << canID <<" " <<  port << " " << value;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VideoClient::start(bool start)
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

void VideoClient::onSokConnected()
{
    if (ui->ipAddress->findText(ui->ipAddress->currentText()) < 0) {
        ui->ipAddress->insertItem(0, ui->ipAddress->currentText());
        saveSettings();
    }
    ui->connectButton->setText(tr("Отсоединить"));
    ui->connectButton->setStyleSheet("background-color:red; color: white;");
}

void VideoClient::onSokDisconnected()
{
    ui->connectButton->setChecked(false);
    ui->connectButton->setText(tr("Соединить"));
//    ui->connectButton->setStyleSheet("background-color:green; color: white;");
}

void VideoClient::onSokDisplayError(QAbstractSocket::SocketError)
{
    QTcpSocket *server = reinterpret_cast<QTcpSocket*>(sender());
    if (server) {
        QMessageBox::critical(0, "Connection Error!", server->errorString(), QMessageBox::Ok);
        server->close();
        onSokDisconnected();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VideoClient::videoStart()
{
    QProcess *process = new QProcess(this);
//    QString program = "mplayer -geometry 0:0 ";
    QString program = "vlc ";
    process->start(program + ui->videoURL->currentText());
    if (process->waitForStarted()) {
        if (ui->videoURL->findText(ui->videoURL->currentText()) < 0) {
            ui->videoURL->insertItem(0, ui->videoURL->currentText());
            saveSettings();
        }
    }
}
