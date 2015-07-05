#ifndef QJOYSTICK_H
#define QJOYSTICK_H

#include <QObject>
#include <QString>
#include <QList>

#ifdef Q_OS_MACX
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif

class QJoystick : public QObject
{
    Q_OBJECT
public:
    QJoystick(QObject *parent = 0);
    ~QJoystick();

    bool init();

    QString joystickName();
    int joystickNumAxes();
    int joystickNumButtons();

    void getData(QList<int> &axis, QList<bool> &buttons);

private:
    SDL_Joystick* m_joystick;
};

#endif // QJOYSTICK_H
