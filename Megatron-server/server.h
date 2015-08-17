#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCheckBox>
#include <QProgressBar>

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

private:
    Ui::Server *ui;
    QTcpServer mServer;
    QTcpSocket *mClient;

    QVector<QCheckBox*> mOutputIndicator;
    QVector<QProgressBar*> mOutputPulseIndicator;

    bool isDriverLoaded;
    void set2057Value(int port, bool isOn);
    void reset2057();
    void set2088Value(int port, int value);
    void reset2088();
};

#endif // SERVER_H
