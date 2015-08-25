#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QCheckBox>
#include <QTimer>
#include "inputbutton.h"
#include <QTcpSocket>
#include "qjoystick.h"

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
    void updateData();
    void connectInput(bool);
    void setLevel(int, bool);
    void setFrequency(int frequency);
    void setLevel(const QVector<int>&);
    void start(bool);
    void onSokReadyRead();
    void onSokConnected();
    void onSokDisconnected();
    void onSokDisplayError(QAbstractSocket::SocketError);
    
private:
    Ui::Client *ui;
    QJoystick *m_joystick;
    QTimer *m_updateTimer;

    QVector<InputButton*> mInputButton;

    QTcpSocket mServer;
    
    void loadSettings();
    void saveSettings();
};

#endif // CLIENT_H
