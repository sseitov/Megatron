#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QCheckBox>
#include <QRadioButton>
#include "controlbutton.h"
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
    void setJoystickAxiz(int, int, int, int);
    void setJoystickButtons(int, bool, bool);
    void clearHistory();
    void addControl();
    void clearControls();
    void switchMode(bool);
    
    void setButton(bool checked);
    void setLevel(int, bool);
    void setLevel(const QVector<int>&);
    void start(bool);
    void onSokConnected();
    void onSokDisconnected();
    void onSokDisplayError(QAbstractSocket::SocketError);
    
private:
    Ui::Client *ui;
    int mCurrentMode;

    QVector<QJoystick*> mJoystick;
    QRadioButton* mModes[NUM_MODE];
    QVector<ControlButton*> mControlButtons[NUM_MODE];
    int mLeftNode[NUM_MODE];
    int mRightNode[NUM_MODE];

    QTcpSocket mServer;
    
    void loadSettings();
    void saveSettings();
    void writeValuesToNode(const QVector<int>& values, int node);
};

#endif // CLIENT_H
