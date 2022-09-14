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
        EmptyCode = 0,
        SendSensorReadings = 1,
        StartSensorReadings = 2,
        StopSensorReadings = 3,
        GetModuleVersion = 4,
        SendModuleVersion = 5,
        EnableLeftTurn = 6,
        EnableRightTurn = 7,
        DisableTurns = 8
    };

    static bool IsRequest(Message &msg);
    static bool IsResponse(Message &msg);
    static bool IsCodeMatch(Message &msg, MessageCodes code);
};