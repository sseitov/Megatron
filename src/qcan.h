#ifndef QCAN_H
#define QCAN_H

#include <QObject>
#include <vs_can_api.h>

class QCAN : public QObject
{
    Q_OBJECT
public:
    explicit QCAN(QObject *parent = 0);

    bool open();
signals:

public slots:

private:
    VSCAN_HANDLE    _handle;
};

#endif // QCAN_H
