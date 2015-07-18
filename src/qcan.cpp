#include "qcan.h"
#include <QDebug>

QCAN::QCAN(QObject *parent) :
    QObject(parent)
{
}

bool QCAN::open()
{//
    _handle = VSCAN_Open("/dev/slcan0", VSCAN_MODE_NORMAL);
    if (_handle < 0) {
        char buf[255];
        VSCAN_GetErrorString(_handle, buf, 255);
        qDebug() << "CAN ERROR:" << buf;
    }
    return false;
}
