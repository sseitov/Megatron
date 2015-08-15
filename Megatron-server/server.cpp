#include "server.h"
#include "ui_server.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(start(bool)));
}

Server::~Server()
{
    delete ui;
}

void Server::start(bool start)
{
    if (start) {
        ui->startButton->setText("Stop");
        ui->startButton->setStyleSheet("QPushButton {color: white;}");
        QList<QHostAddress> list = QNetworkInterface::allAddresses();
        for(int nIter=0; nIter<list.count(); nIter++) {
            if(!list[nIter].isLoopback()) {
                if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ) {
                    setWindowTitle(list[nIter].toString());
                    break;
                }
            }
        }
    } else {
        ui->startButton->setText("Start");
        ui->startButton->setStyleSheet("QPushButton {color: black;}");
        setWindowTitle("Megatron Server");
    }
}
