#ifndef SINEWIDGET_H
#define SINEWIDGET_H

#include <QWidget>

class SineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SineWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    bool doRepaint;
    double mDuty;

signals:

public slots:
    void setPulseDuty(int value);

};

#endif // SINEWIDGET_H
