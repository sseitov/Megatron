#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QCheckBox>
#include "inputbutton.h"
#include <QTcpSocket>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void connectInput(bool);
    void setLevel(int, bool);
    void start(bool);
    void onSokReadyRead();
    void onSokConnected();
    void onSokDisconnected();
    void onSokDisplayError(QAbstractSocket::SocketError);
private:
    Ui::Client *ui;
    QVector<InputButton*> mInputButton;
    QVector<QCheckBox*> mOutputIndicator;

    QTcpSocket mServer;
};

#endif // CLIENT_H
