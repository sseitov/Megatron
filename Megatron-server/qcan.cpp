#include "qcan.h"
#include <QDebug>

extern "C" {
#include "CANOpenShellSlaveOD.h"
}

/***************************  CALLBACK FUNCTIONS  *****************************************/

void Init(CO_Data*, UNS32)
{
}

void Exit(CO_Data*, UNS32)
{
}

void QCan::initialisation(CO_Data*)
{
    qDebug() << "Node_initialisation";
}

void QCan::preOperational(CO_Data*)
{
    qDebug() << "Node_preOperational";
}

void QCan::operational(CO_Data*)
{
    qDebug() << "Node_operational";
}

void QCan::stopped(CO_Data*)
{
    qDebug() << "Node_stopped";
}

void QCan::post_sync(CO_Data*)
{
    qDebug() << "Master_post_sync";
}

void QCan::post_TPDO(CO_Data*)
{
    qDebug() << "Master_post_TPDO";
}

void QCan::post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
    qDebug() << "Slave boot up on node " << nodeid;
    QCan* can = (QCan*)d->classObject;
    emit can->initialized(nodeid);
}

//////////////////////////////////////////////////////////////////////
/// \brief QCan::QCan
/// \param parent
///

QCan::QCan(QObject *parent) : QObject(parent), mPort(0), mData(&CANOpenShellSlaveOD_Data)
{
    // Define callback functions
    mData->classObject = this;
    mData->initialisation = &QCan::initialisation;
    mData->preOperational = &QCan::preOperational;
    mData->operational = &QCan::operational;
    mData->stopped = &QCan::stopped;
    mData->post_sync = &QCan::post_sync;
    mData->post_TPDO = &QCan::post_TPDO;
    mData->post_SlaveBootup = &QCan::post_SlaveBootup;

    pthread_mutex_init(&mMutex, NULL);
    pthread_cond_init(&mOperationFinish, NULL);
}

QCan::~QCan()
{
    if (mPort) {
        // Stop timer thread
        StopTimerLoop(Exit);
        TimerCleanup();
    }
    pthread_mutex_destroy(&mMutex);
    pthread_cond_destroy(&mOperationFinish);
}

bool QCan::init()
{
    if (!LoadCanDriver("/usr/local/lib/libcanfestival_can_vscom.so"))
        return false;

    TimerInit();
    StartTimerLoop(Init);

    s_BOARD Board = {(char*)"/dev/ttyUSB0", (char*)"125K"};
    mPort = canOpen(&Board, mData);
    if(!mPort)
        return false;

    setState(mData, Initialisation);
    masterSendNMTstateChange(mData, 0, NMT_Reset_Node);

    return true;
}

////////////////////////////////////////
// private methods

void QCan::lock()
{
    pthread_mutex_lock(&mMutex);
}

void QCan::wait()
{
    pthread_cond_wait(&mOperationFinish, &mMutex);
    pthread_mutex_unlock(&mMutex);
}

void QCan::signal()
{
    pthread_mutex_lock(&mMutex);
    pthread_cond_signal(&mOperationFinish);
    pthread_mutex_unlock(&mMutex);
}

////////////////////////////////////////
// public methods

int QCan::getCanType(int node)
{
    lock();
    mID = 0;
    readNetworkDictCallback(mData, node, 0x1018, 0x02, 0, &QCan::CheckReadId, 0);
    wait();
    return mID;
}

void QCan::CheckReadId(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data=0;
    UNS32 size=64;

    QCan* can = (QCan*)d->classObject;
    if(getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED) {
        printf("Master : Failed in reading information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    } else {
        can->mID = data;
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
    can->signal();
}

void QCan::CheckWriteSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    QCan* can = (QCan*)d->classObject;
    if(getWriteResultNetworkDict(d, nodeid, &abortCode) != SDO_FINISHED) {
        printf("\nResult : Failed in writing information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
        can->writeError = true;
    } else {
        can->writeError = false;
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
    can->signal();
}

// CAN 2057 methods

void QCan::setTrigger(int node, int port, UNS8 value)
{
    int index = port < 8 ? 1 : 2;
    lock();
    writeNetworkDictCallBack(mData, node, 0x6200, index, 1, 0, &value, &QCan::CheckWriteSDO, 0);
    wait();
}

void QCan::setTriggerPolarity(int node, int port, UNS8 value)
{
    int index = port < 8 ? 1 : 2;
    lock();
    writeNetworkDictCallBack(mData, node, 0x6202, index, 1, 0, &value, &QCan::CheckWriteSDO, 0);
    wait();
}

// CAN 2088 methods

void QCan::setPulseOutput(int node, int port, bool isOn, bool inversion)
{
    UNS8 data = isOn ? 1 : 0;
    lock();
    writeNetworkDictCallBack(mData, node, 0x3100, port+1, 1, 0, &data, &QCan::CheckWriteSDO, 0);
    wait();

    UNS16 value = inversion ? 999 : 1;
    lock();
    writeNetworkDictCallBack(mData, node, 0x3103, port+1, 2, 0, &value, &QCan::CheckWriteSDO, 0);
    wait();
}

void QCan::setPulseFrequency(int node, int port, UNS32 value)
{
    lock();
    writeNetworkDictCallBack(mData, node, 0x3102, port+1, 4, 0, &value, &QCan::CheckWriteSDO, 0);
    wait();
}

void QCan::setPulseDuty(int node, int port, UNS16 value)
{
    if (value < 1 || value > 999) {
        qDebug() << "Invalid value " << value;
        return;
    }
    if (port == 2) {
        qDebug() << "setPulseDuty" << "node = " << node << ", port =" << port << ", value = " << value;
    }
    do {
        lock();
        writeNetworkDictCallBack(mData, node, 0x3103, port+1, 2, 0, &value, &QCan::CheckWriteSDO, 0);
        wait();
    } while (writeError);
}
