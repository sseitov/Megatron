#include "qcan.h"
#include <QDebug>
extern "C" {
#include "ObjDict.h"
}

// Callback function that check the read SDO demand
void CheckReadInfoSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    char data[255];
    UNS32 size=255;

    if(getReadResultNetworkDict(d, nodeid, data, &size, &abortCode) != SDO_FINISHED)
        printf("Master : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    else {
        QCAN* can = (QCAN*)d->classObject;
        emit can->initialized((const char*)data);
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

// Callback function that check the read SDO demand

void CheckHighReadSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data=0;
    UNS32 size=64;

    if(getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    else {
        QCAN* can = (QCAN*)d->classObject;
        emit can->highValues(data);
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

void CheckLowReadSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data=0;
    UNS32 size=64;

    if(getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    else {
        QCAN* can = (QCAN*)d->classObject;
        emit can->lowValues(data);
    }
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

// Callback function that check the write SDO demand
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

/***************************  INITIALISATION  **********************************/

void Init(CO_Data* d, UNS32 id)
{
}

/***************************  CLEANUP  *****************************************/
void Exit(CO_Data* d, UNS32 nodeid)
{
}

/***************************  CALLBACK FUNCTIONS  *****************************************/

void CAN_initialisation(CO_Data* d)
{
    printf("Node_initialisation\n");
}

void CAN_preOperational(CO_Data* d)
{
    printf("Node_preOperational\n");
}

void CAN_operational(CO_Data* d)
{
    printf("Node_operational\n");
}

void CAN_stopped(CO_Data* d)
{
    printf("Node_stopped\n");
}

void CAN_post_sync(CO_Data* d)
{
    printf("Master_post_sync\n");
}

void CAN_post_TPDO(CO_Data* d)
{
    printf("Master_post_TPDO\n");
}

void CAN_post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
    printf("Slave %x boot up\n", nodeid);
}

////////////////////////////////////////////////////
/// \brief QCAN::QCAN
/// \param parent
///
QCAN::QCAN(QObject *parent) :
    QObject(parent), mPort(0), mData(&CANOpenShellSlaveOD_Data), mNodeID(2)
{    
    TimerInit();
}

QCAN::~QCAN()
{
    if (mPort) {
        // Stop timer thread
        StopTimerLoop(&Exit);
        canClose(mData);
    }
    TimerCleanup();
}

bool QCAN::init()
{
    if (!LoadCanDriver("/usr/local/lib/libcanfestival_can_vscom.so"))
        return false;
    s_BOARD Board = {"/dev/ttyUSB0", "125K"};

    /* Define callback functions */
    mData->initialisation = CAN_initialisation;
    mData->preOperational = CAN_preOperational;
    mData->operational = CAN_operational;
    mData->stopped = CAN_stopped;
    mData->post_sync = CAN_post_sync;
    mData->post_TPDO = CAN_post_TPDO;
    mData->post_SlaveBootup = CAN_post_SlaveBootup;
    mData->classObject = this;

    mPort = canOpen(&Board, mData);
    if(!mPort)
        return false;

    // Defining the node Id
    setNodeId(mData, mNodeID);
    // Start Timer thread
    StartTimerLoop(&Init);

    setState(mData, Initialisation);
    masterSendNMTstateChange(mData, mNodeID, NMT_Reset_Node);
    readNetworkDictCallback(mData, mNodeID, 0x1008, 0, 0, CheckReadInfoSDO, 0);

    return true;
}

bool QCAN::started()
{
    return (mPort != 0);
}

void QCAN::writeValue(int value)
{
    int index = value > 0 ? 2 : 1;
    int data = 0;
    if (value != 0) {
        int v = abs(value >> 12);
        if (value < 0) {
            data ^= (0xff >> v);
        } else {
            data = (0xff << v);
        }
        data = 0xff - data;
    }
    if (index == 1) {
        writeNetworkDictCallBack(mData, mNodeID, 0x6200, index, 1, 0, &data, CheckLowWriteSDO, 0);
    } else {
        writeNetworkDictCallBack(mData, mNodeID, 0x6200, index, 1, 0, &data, CheckHighWriteSDO, 0);
    }
}
