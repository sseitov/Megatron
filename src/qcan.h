#ifndef QCAN_H
#define QCAN_H

#include <QObject>
extern "C" {
#include <canfestival/canfestival.h>
}

enum CANBaudRate {
    RateUnknown,
    Rate10K,
    Rate20K,
    Rate50K,
    Rate125K,
    Rate250K,
    Rate500K,
    Rate800K,
    Rate1000K
};

class QCAN : public QObject
{
    Q_OBJECT
public:
    explicit QCAN(QObject *parent = 0);
    ~QCAN();

    static QString rateToString(CANBaudRate);
    static CANBaudRate rateFromString(char* rate);

    bool init();

    QString busName() {
        return "CAN board on " + QString(mBoard.busname);
    }

    CANBaudRate baudRate();
    bool setBaudRate(CANBaudRate rate);

signals:

public slots:
private:
    s_BOARD mBoard;
    CAN_PORT m_port;
};

#endif // QCAN_H
