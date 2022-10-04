#pragma once

#include "models.h"
#include "typedefs.h"


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

// * Create delay between same commands
class Timer
{
private:
    ulong time_from_repeat_ms;
    float repetition_time_sec;
    bool is_enabled;
public:
    Timer(float repetition_time_sec);
    float GetRepeatTime();
    bool IsPassed();
    bool IsEnabled();
    void Enable();
    void Disable();
    void ResetTime();
};