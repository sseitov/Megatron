#ifndef QCAN_H
#define QCAN_H

#include <QObject>
#include <pthread.h>
extern "C" {
#include <canfestival.h>
}

#define CAN_2057    0x2057
#define CAN_2088    0x2088

class QCAN : public QObject
{
    Q_OBJECT
public:
    explicit QCAN(QObject *parent = 0);
    ~QCAN();

    bool init(const char* port);
    int getID();
    const QString& getTitle();
    void resetPwmOutput();

signals:
    void initialized(QCAN*);
    void low2057Values(unsigned int);
    void high2057Values(unsigned int);
    void pwmDuty(int, int);

public slots:
    void writeValue2057(int value);
    void writePwmDuty(int value);
    void togglePwmOutput(bool isOn);

private:
    CAN_PORT    mPort;
    int         mID;
    CO_Data*    mData;
    int         mNodeID;
    QString     mTitle;

    pthread_mutex_t mMutex;
    pthread_cond_t mOperationFinish;
    void lock();
    void wait();
    void signal();

    bool        mPwmChannel[8];
    int         mPwmDuty;

    static void CheckReadId(CO_Data* d, UNS8 nodeid);
    static void CheckReadTitle(CO_Data* d, UNS8 nodeid);
    static void CheckReadPWM(CO_Data* d, UNS8 nodeid);
    static void CheckWriteSDO(CO_Data* d, UNS8 nodeid);

    static void initialisation(CO_Data* d);
    static void preOperational(CO_Data* d);
    static void operational(CO_Data* d);
    static void stopped(CO_Data* d);
    static void post_sync(CO_Data* d);
    static void post_TPDO(CO_Data* d);
    static void post_SlaveBootup(CO_Data* d, UNS8 nodeid);
};

#endif // QCAN_H
