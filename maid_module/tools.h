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
        EMPTY_PREFIX = '?',
        REQUEST = '$',
        RESPONSE = '#'
    };

    enum MessageCodes
    {
        EMPTY_CODE = 0,
        SENSOR_READING_ENTRY = 1,
        START_SENSOR_READINGS_CMD = 2,
        STOP_SENSOR_READINGS_CMD = 3,
        MODULE_VERSION_ENTRY = 4,
        CURRENT_DATETIME_ENTRY = 5,
		    LAUNCH_MODULE_CMD = 7
    };

    /* Codes for development
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
    }; */

    static bool IsRequest(const Message &msg);
    static bool IsResponse(const Message &msg);
    static bool IsCodeMatch(const Message &msg, MessageCodes code);
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
    
    static bool InitializeRom(Sd2Card &card, SdVolume &volume, SdFile &root);
    static uint GetFreeRam(); // bytes
    static uint GetFreeRom(SdVolume &volume); // megabytes
};

// * Getting the number of milliseconds that have elapsed since the header was recorded
class MillisTracker
{
public:
    ulong initialization_time_ms = 0;
    ulong operator ()();
};

// * Event information output for work tracking
class Logger
{
private:
    static const HardwareSerial &LogSerial = Serial;
    static const char* const note = "[LOG]";

    static String MakeLog(String log);
public:
    static void LogTurnStateChanged(const char* turn);
    static void LogMsgSent();
    static void LogMsgRecived();
    static void LogReadingsSaved();
    static void LogHeaderSaved();
};
