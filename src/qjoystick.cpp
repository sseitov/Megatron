#include "qjoystick.h"
#include <QDebug>

#define POLL_INTERVAL 40

QJoystick::QJoystick(QObject *parent) :
    QObject(parent)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
}

QJoystick::~QJoystick()
{
    if (m_joystick) {
        SDL_JoystickClose(m_joystick);
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    this->deleteLater();
}

bool QJoystick::init()
{
    if (SDL_NumJoysticks() < 1) {
        return false;
    } else {
        m_joystick = SDL_JoystickOpen(0);
        return true;
    }
}

QString QJoystick::joystickName()
{
    return QString(SDL_JoystickName(0));
}

int QJoystick::joystickNumAxes()
{
    return (SDL_JoystickNumAxes(m_joystick));
}

int QJoystick::joystickNumButtons()
{
    return (SDL_JoystickNumButtons(m_joystick));
}

void QJoystick::getData(QList<int> &axis, QList<bool> &buttons)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    for(int i=0; i<SDL_JoystickNumAxes(m_joystick); i++)
    {
        axis.append(SDL_JoystickGetAxis(m_joystick,i));
    }

    for(int i=0; i<SDL_JoystickNumButtons(m_joystick); i++)
    {
        buttons.append(SDL_JoystickGetButton(m_joystick,i));
    }
}
