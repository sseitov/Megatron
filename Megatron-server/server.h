#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCheckBox>
#include <QProgressBar>
#include <QTimer>
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
    void saveSettings();
    void connection();
    void slotReadClient();
    void slotDisconnectClient();
    void canInitialized(int);
    void set2057port(int state);

    void onPingerConnected();
    void onPingerDisconnected();
    void onPingerDisplayError(QAbstractSocket::SocketError);
    void ping();
    void slotReadPingAnser();

private:
    Ui::Server *ui;
    QTcpServer mServer;
    QTcpSocket mPinger;
    QTimer mPingTimer;
    bool mPingerConnected;
    int mPingError;
    QCan mCan;

    CANControl mNode2088[3];

    int mNode2057[2];
    QVector<QCheckBox*> mOutputIndicator[2];

    std::bitset<8> m2057lowValue[2];
    std::bitset<8> m2057highValue[2];
    std::bitset<8> m2057lowPolarity[2];
    std::bitset<8> m2057highPolarity[2];

    void set2057Value(int node, int port, bool isOn);
    void start();
    void stop();

    void loadSettings();
    void shutdown();
};

#endif // SERVER_H
