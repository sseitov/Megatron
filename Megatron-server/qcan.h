#ifndef QCAN_H
#define QCAN_H

#include <QObject>
#include <pthread.h>
extern "C" {
#include <canfestival.h>
}

class QCan : public QObject
{
    Q_OBJECT
public:
    explicit QCan(QObject *parent = 0);
    virtual ~QCan();

    bool init();
    int getCanType(int node);

    // CAN-2057
    void setTrigger(int node, int port, UNS8 value);
    void setTriggerPolarity(int node, int port, UNS8 value);

    // CAN-2088
    void setPulseOutput(int node, int port, bool inversion);
    void setPulseFrequency(int node, int port, UNS32 value);
    void setPulseDuty(int node, int port, UNS16 value);

signals:
    void initialized(int);

public slots:

private:
    CAN_PORT    mPort;
    CO_Data*    mData;
    int mID;

    pthread_mutex_t mMutex;
    pthread_cond_t mOperationFinish;

    void lock();
    void wait();
    void signal();

    static void initialisation(CO_Data* d);
    static void preOperational(CO_Data* d);
    static void operational(CO_Data* d);
    static void stopped(CO_Data* d);
    static void post_sync(CO_Data* d);
    static void post_TPDO(CO_Data* d);
    static void post_SlaveBootup(CO_Data* d, UNS8 nodeid);

    static void CheckReadId(CO_Data* d, UNS8 nodeid);
    static void CheckWriteSDO(CO_Data* d, UNS8 nodeid);
};

#endif // QCAN_H
