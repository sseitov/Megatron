#ifndef JOYSTICKMONITOR_H
#define JOYSTICKMONITOR_H

#include <QWidget>
#include <QImage>

#define JOYSTICK_RADIUS  90.0

class JoystickMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit JoystickMonitor(QWidget *parent = 0);
    void setTarget(qreal x, qreal y, qreal z);

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

signals:
    void setLevel(const QVector<int>&);
};

#endif // JOYSTICKMONITOR_H
