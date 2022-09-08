#pragma once

#include "models.h"


// ! Declaring classes from other header files in advance
class Message;


// * Checking inforamtion about transfered message
class MessageAnalyzer
{
public:
    enum MessagePrefixes
    {
        EmptyPrefix = '?',
        Request = '#',
        Response = '$'
    };
    enum MessageCodes
    {
        GetSensorReadings = 0,
        GetModuleVersion = 1,
        SendModuleVersion = 2,
        TurnLeftEnable = 3,
        TurnRightEnable = 4,
        TurnsDisable = 5,
        EmptyCode = 6
    };

    static bool IsRequest(Message msg);
    static bool IsResponse(Message msg);
    static bool IsCodeMatch(Message msg, MessageCodes code);
};