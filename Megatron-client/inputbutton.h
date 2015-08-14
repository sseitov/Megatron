#ifndef INPUTBUTTON_H
#define INPUTBUTTON_H

#include <QGroupBox>
#include <QPushButton>

class InputButton : public QGroupBox
{
    Q_OBJECT
public:
    explicit InputButton(QWidget *parent = 0);
    void connectButton(QPushButton* button);
    void resetConfig();
    void configure(const QString& name, bool checkable, bool inverse, int port);

signals:
    void setLevel(int, bool);

private slots:
    void buttonClicked(bool);
    void levelOn();
    void levelOff();

private:
    QPushButton* mButton;
    bool mInverse;
    int mPort;

};

#endif // INPUTBUTTON_H
