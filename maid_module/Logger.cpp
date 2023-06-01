#include "tools.h"

void Logger::LogTurnStateChanged(const char* turn)
{
    String log; 
    log.reserve(25);

    log = turn;
    log += F(" TURN STATE CHANGED");
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
    String full_log = F("[LOG] ");
    full_log += log;
    full_log += F(" (");
    full_log += millis();
    full_log += F(" ms)");

    return full_log;
}
