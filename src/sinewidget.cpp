#include "sinewidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

SineWidget::SineWidget(QWidget *parent) :
    QWidget(parent)
{
    QPalette Pal(palette());

    // set black background
    Pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(Pal);

    mDuty = 0.5;
    doRepaint = false;
}

void SineWidget::setPulseDuty(int value)
{
    if (value < 0) {
        doRepaint = false;
    } else {
        qDebug() << "SET DUTY " << value;
        doRepaint = true;
        double duty = (double)value/1000.0;
        if (duty <0.001) {
            mDuty = 0.001;
        } else if (duty > 0.999) {
            mDuty = 0.999;
        } else {
            mDuty = duty;
        }
    }
    repaint();
}

void SineWidget::paintEvent(QPaintEvent *pe)
{
    if (doRepaint) {
        int h = pe->rect().height();
        double w = pe->rect().width();
        double firstHalf = w/2*mDuty;

        QPoint points[9] = {
            QPoint(0, h-10),
            QPoint(0, 10),
            QPoint(firstHalf, 10),
            QPoint(firstHalf, h-10),
            QPoint(w/2, h-10),
            QPoint(w/2, 10),
            QPoint(w/2+firstHalf, 10),
            QPoint(w/2+firstHalf, h-10),
            QPoint(w,h-10)
        };

        QPainter painter(this);
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(1);
        painter.setPen(pen);

        painter.drawPolyline(points, 9);
    }
}

