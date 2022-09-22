#include "tools.h"

Repeater::Repeater(float repetition_time_sec)
{
    this-> time_from_repeat_ms = 0;
    this->repetition_time_sec = repetition_time_sec;
}

bool Repeater::IsPassed()
{
    return ((float)millis() - (float)time_from_repeat_ms) / 1000.0 >= repetition_time_sec;
}

void Repeater::ResetTime()
{
    this->time_from_repeat_ms = millis();
}