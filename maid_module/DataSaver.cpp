#include "devices.h"

// * Creating DataSaver class with initial name of file
DataSaver::DataSaver(String readings_filename)
{
    this->date_time = EMPTY_STRING;
    this->readings_filename = readings_filename;
    this->last_write_time = 0;
}

// * Get the last time the data was written to the drive
ulong DataSaver::GetLastWriteTime() const
{
    return this->last_write_time;
}

// * Try to set date of today, get it from app
bool DataSaver::TrySetDateTime(String date_time)
{
   if (date_time.length() != DataSaver::date_time_length)
       return false;

   int time_inx;
   for (int i(0); i < date_time.length(); i++)
   { 
       if (date_time[i] == '-')
       {
           time_inx = i+1;
           break;
       }
           
       else if ((i == 2 || i == 5) && (date_time[i] != '.'))
           return false;

       else if ((i != 2 && i != 5) && !isdigit(date_time[i]))
           return false;
   }
   
   for (int i(time_inx); i < date_time.length(); i++)
   {
       if ((i == time_inx + 2 || i == time_inx + 5) && (date_time[i] != ':'))
           return false;
       
       else if ((i != time_inx + 2 && i != time_inx + 5) && !isdigit(date_time[i]))
           return false;
   }
    
    this->date_time = date_time;
    return true;
}

// * Write all readings to file
void DataSaver::WriteNodes(Node* head)
{
    File readings_file = SD.open(this->readings_filename, FILE_WRITE);
    
    Node* current = head;
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
void DataSaver::WriteHeader(const Voltmeter &voltmeter, const Wheel &wheel, const Timer &save_readings_timer)
{
    const String header = "{H} " + date_time + " ( " + String(wheel.count_of_spokes) + " | " + String(wheel.wheel_circumference_mm) + " | " + String(voltmeter.GetMaxVoltage()) + " | "+ String(save_readings_timer.GetRepeatTime()) + " )";

    File readings_file = SD.open(this->readings_filename, FILE_WRITE);
    readings_file.println(header);
    readings_file.close();

    this->last_write_time = millis();
}
