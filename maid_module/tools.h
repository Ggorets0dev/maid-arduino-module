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
        GetModuleVersion = 2,
        SendModuleVersion = 3,
        EnableLeftTurn = 4,
        EnableRightTurn = 5,
        DisableTurns = 6
    };

    static bool IsRequest(Message &msg);
    static bool IsResponse(Message &msg);
    static bool IsCodeMatch(Message &msg, MessageCodes code);
};