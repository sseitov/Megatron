#ifndef QJOYSTICK_H
#define QJOYSTICK_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>

#ifdef Q_OS_OSX
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif


class QJoystick : public QObject
{
    Q_OBJECT
public:
    QJoystick(int num, SDL_Joystick* joystick);
    ~QJoystick();

private:
    int mID;
    SDL_Joystick* m_joystick;
    QTimer *m_updateTimer;
    int mX;
    int mY;
    bool mButton1;
    bool mButton2;
    
private slots:
    void updateData();
    
signals:
    void setAxiz(int, int, int);
    void setButtons(int, bool, bool);
    
};

#endif // QJOYSTICK_H
