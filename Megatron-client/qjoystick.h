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

    int joystickNumAxes();
    int joystickNumButtons();

    void getData(QList<int> &axis, QList<bool> &buttons);

private:
    int mID;
    SDL_Joystick* m_joystick;
    QTimer *m_updateTimer;
    
private slots:
    void updateData();
    
signals:
    void setData(int, qreal, qreal, bool, bool);
    
};

#endif // QJOYSTICK_H
