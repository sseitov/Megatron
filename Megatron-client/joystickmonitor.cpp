#include "joystickmonitor.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtCore/qmath.h>
#include <QDebug>

JoystickMonitor::JoystickMonitor(QWidget *parent) :
    QWidget(parent)
{
    mTarget = QPoint(0, 0);
    mTargetImage = QImage(":/images/target.png");
    mDragging = false;
    setMouseTracking(true);
}

void JoystickMonitor::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);

    QPoint center = QPoint(pe->rect().width()/2, pe->rect().height()/2);
    painter.drawLine(0, center.y(), pe->rect().width(), center.y());
    painter.drawLine(center.x(), 0, center.x(), pe->rect().height());
    painter.drawRect(0, 0, pe->rect().width()-1, pe->rect().height()-1);

    if (isEnabled()) {
        QPen pen;
        pen.setWidth(4);
        pen.setCapStyle(Qt::RoundCap);
        if (mTarget.y() > 0) {
            pen.setColor(Qt::red);
        } else {
            pen.setColor(Qt::darkCyan);
        }
        
        QVector<qreal> dashes;
        qreal space = 3;
        dashes << 1 << space << 3 << space << 9 << space
        << 27 << space << 9 << space;
        pen.setDashPattern(dashes);
        painter.setPen(pen);
        
        QPoint org = QPoint(mTarget.x()+JOYSTICK_RADIUS, mTarget.y()+JOYSTICK_RADIUS);
        painter.drawLine(center, org);
        
        QRect imageRect = QRect(org.x()-16, org.y()-16, 32, 32);
        painter.drawImage(imageRect, mTargetImage);
    }
}

void JoystickMonitor::mousePressEvent(QMouseEvent *mouse)
{
    if (mouse->button() == Qt::LeftButton) {
        mDragging = true;
        mDragPt = mouse->pos();
    }
}

void JoystickMonitor::mouseMoveEvent(QMouseEvent *mouse)
{
    if (mDragging) {
        QPoint pt = mouse->pos();

        qreal x = mTarget.x() + (pt.x() - mDragPt.x());
        if (x < -JOYSTICK_RADIUS) x = -JOYSTICK_RADIUS;
        if (x > JOYSTICK_RADIUS) x = JOYSTICK_RADIUS;

        qreal y = mTarget.y() + (pt.y() - mDragPt.y());
        if (y < -JOYSTICK_RADIUS) y = -JOYSTICK_RADIUS;
        if (y > JOYSTICK_RADIUS) y = JOYSTICK_RADIUS;
/*
        if (qPow(x,2)+qPow(y,2) > qPow(150,2)) {
            qreal alpha = qAtan2(y, x);
            x = qCos(alpha)*150.0;
            y = qSin(alpha)*150.0;
        }
*/
        setTarget(x, y, 0);

        mDragPt = pt;
        repaint();
    }
}

void JoystickMonitor::mouseReleaseEvent(QMouseEvent *mouse)
{
    if (mouse->button() == Qt::LeftButton) {
        mDragging = false;
    }
}

void JoystickMonitor::setTarget(qreal x, qreal y, qreal z)
{
    mTarget.setX(x);
    mTarget.setY(y);
    repaint();

    qreal port0, port1, port2, port3, port4, port5;
    if (y < 0) {
        port0 = fabs(y)/JOYSTICK_RADIUS*1000.0;
        port1 = 0;
    } else {
        port1 = fabs(y)/JOYSTICK_RADIUS*1000.0;
        port0 = 0;
    }
    if (x < 0) {
        port2 = fabs(x)/JOYSTICK_RADIUS*1000.0;
        port3 = 0;
    } else {
        port3 = fabs(x)/JOYSTICK_RADIUS*1000.0;
        port2 = 0;
    }
    if (z < 0) {
        port4 = fabs(z)/JOYSTICK_RADIUS*1000.0;
        port5 = 0;
    } else {
        port5 = fabs(z)/JOYSTICK_RADIUS*1000.0;
        port4 = 0;
    }

    QVector<int> values;
    values.append(port0);
    values.append(port1);
    values.append(port2);
    values.append(port3);
    values.append(port4);
    values.append(port5);
    emit setLevel(values);
}
