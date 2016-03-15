#ifndef COMMON_H
#define COMMON_H

#define SERVER_SOCKET   1964
#define PING_SOCKET   1402

enum CANType {
    CAN_None,
    CAN_2057,
    CAN_2088
};

enum CommandType {
    CAN_Acknowledge,
    CAN_Alive,
    CAN_SetValue
};

#endif // COMMON_H
