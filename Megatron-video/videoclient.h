#ifndef VIDEOCLIENT_H
#define VIDEOCLIENT_H

#include <QWidget>

namespace Ui {
class VideoClient;
}

class VideoClient : public QWidget
{
    Q_OBJECT

public:
    explicit VideoClient(QWidget *parent = 0);
    ~VideoClient();

private:
    Ui::VideoClient *ui;
};

#endif // VIDEOCLIENT_H
