#include "devices.h"

Logging::Logging(String logs_filename, String blocks_filename)
{
    this->now_date = EMPTY_STRING;
    this->logs_filename = logs_filename;
    this->blocks_filename = blocks_filename;
}

void Logging::SetDate(String date)
{
    this->now_date = date;
}

bool Logging::IsDateAvailable()
{
    return now_date != EMPTY_STRING;
}

void Logging::WriteBlocks(Node* head)
{
    Node* current = head;
    File blocks_file = SD.open(blocks_filename, FILE_WRITE);
    String reading;

    while (current != NULL)
    {
        reading = "{R} " + String(current->time) + " | " + String(current->speed_kmh, 2) + " " + String(current->voltage_v, 2);
        blocks_file.println(reading);
    }

    blocks_file.close();
}

bool Logging::WriteHeader(Wheel &wheel, Timer &save_readings_timer)
{
    if (now_date == EMPTY_STRING)
        return false;

    File blocks_file = SD.open(blocks_filename);

    String header = "{H} " + now_date + " (" + String(wheel.GetSpokesCount()) + " / " + String(wheel.GetWheelCircumference()) + " / " + String(save_readings_timer.GetRepeatTime()) + " )";
    blocks_file.println(header);

    blocks_file.close();

    return true;
}

void Logging::Log(LogType type, String msg)
{
	File log_file = SD.open(logs_filename, FILE_WRITE);
    String log = String(millis()) + " | " + String(type) + " | " + msg;

    if (log_file)
        log_file.println(log);
    
    log_file.close();
}
