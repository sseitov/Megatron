#include "qcan.h"
#include <QDebug>

extern "C" {
#include "2088C.h"
}

static const char* gLibraryPath = "/usr/local/lib/libcanfestival_can_serial.so";

QCAN::QCAN(QObject *parent) :
    QObject(parent)
{
    mBoard.busname = (char*)"/dev/ttyUSB0";
    mBoard.baudrate = (char*)"125K";
}

QCAN::~QCAN()
{
}

bool QCAN::init()
{
    if (LoadCanDriver(gLibraryPath) == NULL) {
        qDebug() << "Unable to load library: " << gLibraryPath;
        return false;
    }
    m_port = canOpen(&mBoard,&_Data);
    if(!m_port){
        qDebug() << "Cannot open CAN Board " << mBoard.busname << " with rate " << mBoard.baudrate;
        return false;
    }
    setNodeId(&_Data, 0x01);
    setState(&_Data, Initialisation);
    e_nodeState state = getState(&_Data);
    if (state = Pre_operational) {
        setState(&_Data, Operational);
        qDebug() << "Current state " << getState(&_Data);
    }
    return true;
}

QString QCAN::rateToString(CANBaudRate rate)
{
    switch (rate) {
    case Rate10K:
        return QString("10K");
    case Rate20K:
        return QString("20K");
    case Rate50K:
        return QString("50K");
    case Rate125K:
        return QString("125K");
    case Rate250K:
        return QString("250K");
    case Rate500K:
        return QString("500K");
    case Rate800K:
        return QString("800K");
    case Rate1000K:
        return QString("100K");
    default:
        return QString("Unknown");
    }
}

CANBaudRate QCAN::rateFromString(char* rate)
{
    if (strcmp("10K", rate) == 0) {
        return Rate10K;
    } else if (strcmp("20K", rate) == 0) {
        return Rate20K;
    } else if (strcmp("50K", rate) == 0) {
        return Rate50K;
    } else if (strcmp("125K", rate) == 0) {
        return Rate125K;
    } else if (strcmp("250K", rate) == 0) {
        return Rate250K;
    } else if (strcmp("500K", rate) == 0) {
        return Rate500K;
    } else if (strcmp("800K", rate) == 0) {
        return Rate800K;
    } else if (strcmp("1000K", rate) == 0) {
        return Rate1000K;
    } else {
        return RateUnknown;
    }
}

CANBaudRate QCAN::baudRate()
{
    return QCAN::rateFromString(mBoard.baudrate);
}

bool QCAN::setBaudRate(CANBaudRate rate)
{
    UNS8 err = canChangeBaudRate(m_port, (char*)QCAN::rateToString(rate).toStdString().c_str());
    return (err == 0);
}
