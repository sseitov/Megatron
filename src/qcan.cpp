#include "qcan.h"
#include <QDebug>

extern "C" {
#include "ObjDict.h"
}

/***************************  CALLBACK FUNCTIONS  *****************************************/

void QCAN::initialisation(CO_Data*)
{
    qDebug() << "Node_initialisation";
}

void QCAN::preOperational(CO_Data*)
{
    qDebug() << "Node_preOperational";
}

void QCAN::operational(CO_Data*)
{
    qDebug() << "Node_operational";
}

void QCAN::stopped(CO_Data*)
{
    qDebug() << "Node_stopped";
}

void QCAN::post_sync(CO_Data*)
{
    qDebug() << "Master_post_sync";
}

void QCAN::post_TPDO(CO_Data*)
{
    qDebug() << "Master_post_TPDO";
}

void QCAN::post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
    qDebug() << "Slave boot up on node " << nodeid;
    QCAN* can = (QCAN*)d->classObject;
    emit can->initialized(can);
}

// Callback function that check the read 2057 SDO demand

void CheckHighReadSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data=0;
    UNS32 size=sizeof(data);

    if(getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    else {
        QCAN* can = (QCAN*)d->classObject;
        emit can->high2057Values(data);
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

void CheckLowReadSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data=0;
    UNS32 size=sizeof(data);

    if(getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    else {
        QCAN* can = (QCAN*)d->classObject;
        emit can->low2057Values(data);
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

// Callback function that check the write 2057 SDO demand

void CheckLowWriteSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    if(getWriteResultNetworkDict(d, nodeid, &abortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
    readNetworkDictCallback(d, nodeid, 0x6200, 1, 0, CheckLowReadSDO, 0);
}

void CheckHighWriteSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    if(getWriteResultNetworkDict(d, nodeid, &abortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
    readNetworkDictCallback(d, nodeid, 0x6200, 2, 0, CheckHighReadSDO, 0);
}

////////////////////////////////////////////////////
/// \brief QCAN::QCAN
/// \param parent
///
QCAN::QCAN(QObject *parent) :
    QObject(parent), mPort(0), mID(0), mNodeID(2)
{
    mData = (CO_Data*)malloc(sizeof(CANOpenShellSlaveOD_Data));
    memcpy(mData, &CANOpenShellSlaveOD_Data, sizeof(CANOpenShellSlaveOD_Data));
    pthread_mutex_init(&mMutex, NULL);
    pthread_cond_init(&mOperationFinish, NULL);
}

QCAN::~QCAN()
{
    if (mPort) {
        resetPwmOutput();
        canClose(mData);
    }
    free(mData);
    pthread_mutex_destroy(&mMutex);
    pthread_cond_destroy(&mOperationFinish);
}

bool QCAN::init(const char* port)
{
    if (!LoadCanDriver("/usr/local/lib/libcanfestival_can_vscom.so"))
        return false;
    s_BOARD Board = {(char*)port, (char*)"125K"};

    /* Define callback functions */
    mData->classObject = this;
    mData->initialisation = &QCAN::initialisation;
    mData->preOperational = &QCAN::preOperational;
    mData->operational = &QCAN::operational;
    mData->stopped = &QCAN::stopped;
    mData->post_sync = &QCAN::post_sync;
    mData->post_TPDO = &QCAN::post_TPDO;
    mData->post_SlaveBootup = &QCAN::post_SlaveBootup;

    mPort = canOpen(&Board, mData);
    if(!mPort)
        return false;

    // Defining the node Id
    setNodeId(mData, mNodeID);
    setState(mData, Initialisation);
    masterSendNMTstateChange(mData, mNodeID, NMT_Reset_Node);

    return true;
}

int QCAN::getID()
{
    lock();
    readNetworkDictCallback(mData, mNodeID, 0x1018, 0x02, 0, &QCAN::CheckReadId, 0);
    wait();
    return mID;
}

const QString& QCAN::getTitle()
{
    lock();
    readNetworkDictCallback(mData, mNodeID, 0x1008, 0, 0, &QCAN::CheckReadTitle, 0);
    wait();
    return mTitle;
}

void QCAN::resetPwmOutput()
{
    for (int i=0; i<8; i++) {
        if (mPwmChannel[i]) {
            mPwmChannel[i] = false;
            UNS8 data = 0;
            lock();
            writeNetworkDictCallBack(mData, mNodeID, 0x3100, i+1, 1, 0, &data, &QCAN::CheckWriteSDO, 0);
            wait();
        }
    }
}

void QCAN::writeValue2057(int value)
{
    int index = value > 0 ? 2 : 1;
    int valueIndex = abs(value) / 4096;

    unsigned int data = 0;
    if (index == 1) {
        int values[9] = {0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, };
        data = values[8-valueIndex];
        writeNetworkDictCallBack(mData, mNodeID, 0x6200, index, 1, 0, &data, CheckLowWriteSDO, 0);
    } else {
        int values[9] = {0, 0x01, 0x03, 0x07, 0x0f, 0x1F, 0x3F, 0x7F, 0xfF};
        data = values[valueIndex];
        writeNetworkDictCallBack(mData, mNodeID, 0x6200, index, 1, 0, &data, CheckHighWriteSDO, 0);
    }
}

void QCAN::writePwmDuty(int value)
{
    UNS16 data = 500 + ((double)value/32768.0)*500;
    for (int channel = 0; channel < 8; channel++) {
        mPwmDuty = -1;
        if (mPwmChannel[channel]) {
            lock();
            writeNetworkDictCallBack(mData, mNodeID, 0x3103, channel+1, 2, 0, &data, &QCAN::CheckWriteSDO, 0);
            wait();

            lock();
            readNetworkDictCallback(mData, mNodeID, 0x3103, channel+1, 0, &QCAN::CheckReadPWM, 0);
            wait();
        }
        emit pwmDuty(channel, mPwmDuty);
    }
}

void QCAN::togglePwmOutput(bool isOn)
{
    int channel = sender()->objectName().toInt();
    mPwmChannel[channel] = isOn;
    UNS8 data = isOn ? 1 : 0;
    lock();
    writeNetworkDictCallBack(mData, mNodeID, 0x3100, channel+1, 1, 0, &data, &QCAN::CheckWriteSDO, 0);
    wait();


    mPwmDuty = -1;
    if (isOn) {
        lock();
        readNetworkDictCallback(mData, mNodeID, 0x3103, channel+1, 0, &QCAN::CheckReadPWM, 0);
        wait();
    }
    emit pwmDuty(channel, mPwmDuty);
}

////////////////////////////////////////
// private methods

void QCAN::lock()
{
    pthread_mutex_lock(&mMutex);
}

void QCAN::wait()
{
    pthread_cond_wait(&mOperationFinish, &mMutex);
    pthread_mutex_unlock(&mMutex);
}

void QCAN::signal()
{
    pthread_mutex_lock(&mMutex);
    pthread_cond_signal(&mOperationFinish);
    pthread_mutex_unlock(&mMutex);
}

////////////////////////////////////////
// private static methods

void QCAN::CheckReadId(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data=0;
    UNS32 size=64;

    QCAN* can = (QCAN*)d->classObject;
    if(getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED) {
        printf("Master : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    } else {
        can->mID = data;
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
    can->signal();
}

void QCAN::CheckReadTitle(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    char data[255];
    UNS32 size=sizeof(data);

    QCAN* can = (QCAN*)d->classObject;
    if(getReadResultNetworkDict(d, nodeid, data, &size, &abortCode) != SDO_FINISHED) {
        printf("Master : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    } else {
        can->mTitle = QString((const char*)data);
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
    can->signal();
}

void QCAN::CheckReadPWM(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS16 data;
    UNS32 size=sizeof(data);

    QCAN* can = (QCAN*)d->classObject;
    if(getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED) {
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    } else {
        can->mPwmDuty = data;
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
    can->signal();
}

void QCAN::CheckWriteSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    QCAN* can = (QCAN*)d->classObject;
    if(getWriteResultNetworkDict(d, nodeid, &abortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
    can->signal();
}
