#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QCheckBox>
#include "inputbutton.h"
#include <QTcpSocket>
#include "qjoystick.h"

namespace Ui {
class Client;
}

#define NUM_MODE    4

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void setJoystickData(int, qreal, qreal, bool, bool);
    void clearHistory();
    void addControl();
    void clearControls();
    void connectInput(bool);
    void setLevel(int, bool);
    void setLevel(const QVector<int>&);
    void start(bool);
    void onSokReadyRead();
    void onSokConnected();
    void onSokDisconnected();
    void onSokDisplayError(QAbstractSocket::SocketError);
    
private:
    Ui::Client *ui;

    QVector<InputButton*> mInputButton;
    QVector<QJoystick*> mJoystick;

    QTcpSocket mServer;
    
    void loadSettings();
    void saveSettings();
};

#endif // CLIENT_H
