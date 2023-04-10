#include "tools.h"

void Logger::LogTurnStateChanged(const char* turn)
{
    const String log = String(turn) + " TURN STATE CHANGED";
    this->LogSerial.println(MakeLog(log));
}

void Logger::LogMsgSent()
{
    this->LogSerial.println(MakeLog("MESSAGE SENT"));
}

void Logger::LogMsgRecived()
{
    this->LogSerial.println(MakeLog("MESSAGE RECIVED"));
}

void Logger::LogReadingsSaved()
{
    this->LogSerial.println(MakeLog("READINGS ARE SAVED"));
}

void Logger::LogHeaderSaved()
{
    this->LogSerial.println(MakeLog("HEADER IS SAVED"));
}

String Logger::MakeLog(String log)
{
    const String time_passed = String(millis());
    const String full_log = "[LOG] " + log + " (" + time_passed + " ms)";

    return full_log;
}
