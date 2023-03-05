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

    // Codes when developing
    enum MessageCodes
    {
        EmptyCode = 0,
        SendSensorReadings = 1,
        StartSensorReadings = 2,
        StopSensorReadings = 3,
        SendModuleVersion = 4,
        EnableLeftTurn = 5,
        EnableRightTurn = 6,
        Alert = 7,
        DisableTurns = 8,
        GetNowDate = 9,
		    InitializationDone = 10
    };

    /* Codes in the release version
    enum MessageCodes
    {
        EmptyCode = 0,
        SensorReadingEntry = 1,
        StartSensorReadingsCmd = 2,
        StopSensorReadingsCmd = 3,
        ModuleVersionEntry = 4,
        CurrentDateTimeEntry = 5,
		    ModuleLaunchCmd = 6
    };
    */

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
    float GetRepeatTime() const;
    bool IsPassed() const;
    bool IsEnabled() const;
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

// * Getting the number of milliseconds that have elapsed since the header was recorded
class MillisTracker
{
public:
    ulong initialization_time_ms = 0;
    ulong operator ()();
};
