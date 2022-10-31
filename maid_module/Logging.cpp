#include "devices.h"

// * Creating Logging class with initial name of file
Logging::Logging(String readings_filename)
{
    this->now_date = EMPTY_STRING;
    this->readings_filename = readings_filename;
    this->last_write_time = 0;
}

ulong Logging::GetLastWriteTime() const
{
    return this->last_write_time;
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

    return true;
}

// * Write all readings to file
void Logging::WriteNodes(Node* head)
{
    Node* current = head;
    File readings_file = SD.open(readings_filename, FILE_WRITE);
    String reading;

    while (current != NULL)
    {
        reading = "{R} " + String(current->time) + " | " + String(current->impulse_cnt) + " | " + String(current->analog_voltage);
        readings_file.println(reading);
        current = current->next;
    }

    readings_file.close();

    this->last_write_time = millis();
}

// * Create header of readings in file
void Logging::WriteHeader(Voltmeter &voltmeter, Wheel &wheel, Timer &save_readings_timer)
{
    File readings_file = SD.open(readings_filename, FILE_WRITE);

    String header = "{H} " + now_date + " ( " + String(wheel.count_of_spokes) + " | " + String(wheel.wheel_circumference_mm) + " | " + String(voltmeter.GetMaxVoltage()) + " | "+ String(save_readings_timer.GetRepeatTime()) + " )";
    readings_file.println(header);

    readings_file.close();

    this->last_write_time = millis();
}
