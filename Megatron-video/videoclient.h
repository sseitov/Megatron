#ifndef VIDEOCLIENT_H
#define VIDEOCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include "controlbutton.h"

namespace Ui {
class VideoClient;
}

class VideoClient : public QWidget
{
    Q_OBJECT

public:
    explicit VideoClient(QWidget *parent = 0);
    ~VideoClient();
    
private slots:
    void clearHistory();
    void clearVideoHistory();
    void addControl();
    void clearControls();

    void setLevel(int, bool);
    
    void start(bool);
    void onSokConnected();
    void onSokDisconnected();
    void onSokDisplayError(QAbstractSocket::SocketError);

    void videoStart();

private:
    Ui::VideoClient *ui;
    QVector<ControlButton*> mControlButtons;

    QTcpSocket mServer;
    
    void loadSettings();
    void saveSettings();

};

#endif // VIDEOCLIENT_H
