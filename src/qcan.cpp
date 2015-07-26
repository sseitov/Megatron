#include "qcan.h"
#include <QDebug>
extern "C" {
#include "ObjDict.h"
}

/***************************  INITIALISATION  **********************************/

// Callback function that check the read SDO demand
void CheckReadInfoSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data=0;
    UNS32 size=64;

    if(getReadResultNetworkDict(d, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
        printf("Master : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    else {
        QCAN* can = (QCAN*)d->classObject;
        emit can->initialized(QString::number(data, 16));
    }

    // Finalize last SDO transfer with this node
    closeSDOtransfer(d, nodeid, SDO_CLIENT);
}

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
    readNetworkDictCallback(mData, mNodeID, 0x1018, 0x02, 0, CheckReadInfoSDO, 0);

    return true;
}
