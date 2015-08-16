#ifndef INPUTBUTTON_H
#define INPUTBUTTON_H

#include <QGroupBox>
#include <QPushButton>

struct ButtonConfig {
    ButtonConfig() : name(""), checkable(false), inverse(false), port(-1) {}
    QString name;
    bool checkable;
    bool inverse;
    int port;
};

class InputButton : public QGroupBox
{
    Q_OBJECT
public:
    explicit InputButton(QWidget *parent = 0);
    void connectButton(QPushButton* button);
    void setConfig(const ButtonConfig& config);
    void resetConfig();
    const ButtonConfig& config() { return mConfig; }
    void uncheck();

signals:
    void setLevel(int, bool);

private slots:
    void buttonClicked(bool);
    void levelOn();
    void levelOff();

private:
    QPushButton* mButton;
    ButtonConfig mConfig;
    void sendLevel(bool);
};

#endif // INPUTBUTTON_H
