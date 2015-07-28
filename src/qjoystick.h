#ifndef QJOYSTICK_H
#define QJOYSTICK_H

#include <QObject>
#include <QString>
#include <QList>

#include <SDL/SDL.h>

class QJoystick : public QObject
{
    Q_OBJECT
public:
    QJoystick(QObject *parent = 0);
    ~QJoystick();

    bool init();
    bool started();

    QString joystickName();
    int joystickNumAxes();
    int joystickNumButtons();

    void getData(QList<int> &axis, QList<bool> &buttons);

private:
    SDL_Joystick* m_joystick;
};

#endif // QJOYSTICK_H
