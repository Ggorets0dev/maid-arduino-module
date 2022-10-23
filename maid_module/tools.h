#pragma once

#include <SD.h>
#include <SPI.h>
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
        Request = '$',
        Response = '#'
    };
    enum MessageCodes
    {
        EmptyCode = 0,
        SendSensorReadings = 1,
        StartSensorReadings = 2,
        StopSensorReadings = 3,
        SendModuleVersion = 4,
        EnableLeftTurn = 5,
        EnableRightTurn = 6,
        DisableTurns = 7,
        GetNowDate = 8,
		InitializationDone = 9
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
    Timer();
    float GetRepeatTime();
    bool IsPassed();
    bool IsEnabled();
    void Enable();
    void Disable();
    void ResetTime();
};

// * Responsible for monitoring the consumption of RAM and ROM
class Memory
{
public:
    static const byte minimal_free_ram_size = 75; // bytes
    static const byte minimal_free_rom_size = 10; // megabytes
    
    static bool InitROM(Sd2Card &card, SdVolume &volume, SdFile &root);
    static uint GetFreeRAM(); // bytes
    static uint GetFreeROM(SdVolume &volume); // megabytes
};
