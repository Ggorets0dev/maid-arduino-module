#include "tools.h"

void Logger::LogTurnStateChanged(const char* turn)
{
    const String log = String(turn) + " TURN STATE CHANGED";
    Logger::LogSerial.println(MakeLog(log));
}

void Logger::LogMsgSent()
{
    Logger::LogSerial.println(MakeLog("MESSAGE SENT"));
}

void Logger::LogMsgRecived()
{
    Logger::LogSerial.println(MakeLog("MESSAGE RECIVED"));
}

void Logger::LogReadingsSaved()
{
    Logger::LogSerial.println(MakeLog("READINGS ARE SAVED"));
}

void Logger::LogHeaderSaved()
{
    Logger::LogSerial.println(MakeLog("HEADER IS SAVED"));
}

String Logger::MakeLog(String log)
{
    const String time_passed = String(millis());
    const String note = String(Logger::note);
    const String full_log = note + " " + log + " (" + time_passed + " ms)";

    return full_log;
}