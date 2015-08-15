#ifndef JOYSTICKMONITOR_H
#define JOYSTICKMONITOR_H

#include <QWidget>
#include <QImage>

class JoystickMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit JoystickMonitor(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * mouse);
    void mouseMoveEvent(QMouseEvent * mouse);
    void mouseReleaseEvent(QMouseEvent * mouse);

private:
    QPoint mTarget;
    QImage mTargetImage;
    QPoint mDragPt;
    bool mDragging;

    void setTarget(qreal x, qreal y);

signals:
    void setLevel(int, int, int, int);
};

#endif // JOYSTICKMONITOR_H
