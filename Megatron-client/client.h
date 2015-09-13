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
    void setJoystickData(int, qreal, qreal, bool, bool);
    void clearHistory();
    void addControl();
    void clearControls();
    void switchMode(bool);
    void setFrequency(int frequency);
    void setInversion(bool isOn);
    void setLowLevel(int);
    void setHighLevel(int);

    void setLevel(int, bool);
    void setLevel(const QVector<int>&);
    void start(bool);
    void onSokReadyRead();
    void onSokConnected();
    void onSokDisconnected();
    void onSokDisplayError(QAbstractSocket::SocketError);
    
private:
    Ui::Client *ui;
    int mCurrentMode;

    QVector<QJoystick*> mJoystick;
    QRadioButton* mModes[NUM_MODE];
    QVector<ControlButton*> mControlButtons[NUM_MODE];
    int mLeftFrequency[NUM_MODE];
    int mRightFrequency[NUM_MODE];
    bool mLeftInversion[NUM_MODE];
    bool mRightInversion[NUM_MODE];
    int mLeftTopLevel[NUM_MODE];
    int mLeftLowLevel[NUM_MODE];
    int mRightTopLevel[NUM_MODE];
    int mRightLowLevel[NUM_MODE];
    int mLeftNode[NUM_MODE];
    int mRightNode[NUM_MODE];

    QTcpSocket mServer;
    
    void loadSettings();
    void saveSettings();
};

#endif // CLIENT_H
