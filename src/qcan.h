#ifndef QCAN_H
#define QCAN_H

#include <QObject>
extern "C" {
#include <canfestival.h>
}

class QCAN : public QObject
{
    Q_OBJECT
public:
    explicit QCAN(QObject *parent = 0);
    ~QCAN();

    bool init();

signals:
    void initialized(const QString&);

public slots:

private:
    CAN_PORT    mPort;
    CO_Data*    mData;
    int         mNodeID;
};

#endif // QCAN_H
