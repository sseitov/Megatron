#include "qjoystick.h"
#include <QDebug>

#define POLL_INTERVAL 100

QJoystick::QJoystick(int num, SDL_Joystick* joystick) :
    QObject(0), mID(num), m_joystick(joystick), mX(0), mY(0), mButton1(false), mButton2(false)
{
    m_updateTimer = new QTimer;
    m_updateTimer->setInterval(POLL_INTERVAL);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    m_updateTimer->start();
}

QJoystick::~QJoystick()
{
    m_updateTimer->stop();
    delete m_updateTimer;
    SDL_JoystickClose(m_joystick);
}

void QJoystick::updateData()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    
    qreal rx = SDL_JoystickGetAxis(m_joystick, 0);
    qreal ry = SDL_JoystickGetAxis(m_joystick, 1);
    
    int x = rx/32765.0*100;
    int y = ry/32765.0*100;
    
    if (abs(x) < 3) {
        x = 0;
    }
    if (abs(y) < 3) {
        y = 0;
    }
    
    bool b1 = SDL_JoystickGetButton(m_joystick, 0);
    bool b2 = SDL_JoystickGetButton(m_joystick, 1);
    
    if (x != mX || y != mY) {
        emit setAxiz(mID, x, y);
    }
    if (b1 != mButton1 || b2 != mButton2) {
        emit setButtons(mID, b1, b2);
    }
    
    mX = x;
    mY = y;
    mButton1 = b1;
    mButton2 = b2;
}
