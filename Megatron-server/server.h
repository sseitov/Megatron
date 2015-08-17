#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCheckBox>
#include <QSlider>
#include "qcan.h"
#include <bitset>

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
    void start(bool);
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
    int mNode2088;

    QVector<QCheckBox*> mOutputIndicator;
    QVector<QSlider*> mOutputPulseIndicator;


    std::bitset<8> m2057lowValue;
    std::bitset<8> m2057highValue;
    std::bitset<8> m2057lowPolarity;
    std::bitset<8> m2057highPolarity;

    void set2057Value(int port, bool isOn);
    void reset2057();
    void set2088Value(int port, int value);
    void reset2088();
};

#endif // SERVER_H
