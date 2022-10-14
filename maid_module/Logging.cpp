#include "devices.h"

// * Creating Logging class with initial names of files
Logging::Logging(String logs_filename, String blocks_filename)
{
    this->now_date = EMPTY_STRING;
    this->logs_filename = logs_filename;
    this->blocks_filename = blocks_filename;
}

// * Try to set date of today, get it from app
bool Logging::TrySetDate(String date)
{ 
    if (date.length() != 10)
        return false;

    for (int i(0); i < date.length(); i++)
    { 
        if ((i == 2 || i == 5) && (date[i] != '.'))
            return false;

        else if ((i != 2 && i != 5) && !isDigit(date[i]))
            return false;
    }

    this->now_date = date;

    Log(Logging::LogType::Success, "Date was successfully retrieved and stored");

    return true;
}

// * Write all readings to file
void Logging::WriteBlocks(Node* head)
{
    Node* current = head;
    File blocks_file = SD.open(blocks_filename, FILE_WRITE);
    String reading;

    while (current != NULL)
    {
        reading = "{R} " + String(current->time) + " | " + String(current->speed_kmh, 2) + " " + String(current->voltage_v, 2);
        blocks_file.println(reading);
        current = current->next;
    }

    blocks_file.close();

    Log(Logging::LogType::Info, "Another list of nodes was written to the file");
}

// * Create header of readings in file
void Logging::WriteHeader(Wheel &wheel, Timer &save_readings_timer)
{
    File blocks_file = SD.open(blocks_filename, FILE_WRITE);

    String header = "{H} " + now_date + " (" + String(wheel.GetSpokesCount()) + " / " + String(wheel.GetWheelCircumference()) + " / " + String(save_readings_timer.GetRepeatTime()) + " )";
    blocks_file.println(header);

    blocks_file.close();

    Log(Logging::LogType::Success, "Header was successfully created and written");
}

// * Log event in file
void Logging::Log(LogType type, String msg)
{
	File log_file = SD.open(logs_filename, FILE_WRITE);
    String log = String(millis()) + " | " + String(type) + " | " + msg;

    if (now_date != EMPTY_STRING)
        log = now_date + " " + log;

    if (log_file)
        log_file.println(log);
    
    log_file.close();
}
