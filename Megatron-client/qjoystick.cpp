#include "qjoystick.h"
#include <QDebug>

#define POLL_INTERVAL 40

QJoystick::QJoystick(int num, SDL_Joystick* joystick) :
    QObject(0), mID(num), m_joystick(joystick)
{
    m_updateTimer = new QTimer;
    m_updateTimer->setInterval(50);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    m_updateTimer->start();
}

QJoystick::~QJoystick()
{
    m_updateTimer->stop();
    delete m_updateTimer;
    SDL_JoystickClose(m_joystick);
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

void QJoystick::updateData()
{
    QList<int> axis;
    QList<bool> buttons;
    getData(axis, buttons);
    
    qreal x = axis[0];
    qreal y = axis[1];
    bool b1 = buttons[0];
    bool b2 = buttons[1];
    
    emit setData(mID, x, y, b1, b2);
}
