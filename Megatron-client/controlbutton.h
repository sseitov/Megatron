#ifndef CONTROLBUTTON_H
#define CONTROLBUTTON_H

#include <QPushButton>

struct ButtonConfig {
    ButtonConfig() : name(""), checkable(false), inverse(false), port(-1) {}
    QString name;
    bool checkable;
    bool inverse;
    int port;
};

class ControlButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ControlButton(QWidget *parent = 0);
    void setConfig(const ButtonConfig& config);
    const ButtonConfig& config() { return mConfig; }

signals:
    void setLevel(int, bool);

private slots:
    void buttonClicked(bool);
    void levelOn();
    void levelOff();

private:
    ButtonConfig mConfig;
    void sendLevel(bool);
};

#endif // CONTROLBUTTON_H
