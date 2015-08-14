#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QCheckBox>
#include "inputbutton.h"

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

private:
    Ui::Client *ui;
    QVector<InputButton*> mInputButton;
    QVector<QCheckBox*> mOutputIndicator;
};

#endif // CLIENT_H
