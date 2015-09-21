#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCheckBox>
#include <QProgressBar>
#include "qcan.h"
#include <bitset>
#include "cancontrol.h"

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private slots:
    void connection();
    void slotReadClient();
    void slotDisconnectClient();
    void canInitialized(int);
    void set2057port(int state);

private:
    Ui::Server *ui;
    QTcpServer mServer;
    QTcpSocket *mClient;
    QCan mCan;
    int mNode2057;
    QVector<QCheckBox*> mOutputIndicator;

    CANControl mNode2088[3];


    std::bitset<8> m2057lowValue;
    std::bitset<8> m2057highValue;
    std::bitset<8> m2057lowPolarity;
    std::bitset<8> m2057highPolarity;

    void set2057Value(int port, bool isOn);
    void reset2057();

    void loadSettings();
    void saveSettings();
};

#endif // SERVER_H
