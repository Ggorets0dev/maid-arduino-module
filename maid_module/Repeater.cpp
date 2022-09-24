#include "tools.h"

// * Creating a repeater class with delay
Repeater::Repeater(float repetition_time_sec)
{
    this->time_from_repeat_ms = 0;
    this->repetition_time_sec = repetition_time_sec;
}

// * Getting delay from class for calculations
float Repeater::GetRepeatTime()
{
    return this->repetition_time_sec;
}

// * Checking if delay is ended
bool Repeater::IsPassed()
{
    return ((float)millis() - (float)time_from_repeat_ms) / 1000.0 >= repetition_time_sec;
}

// * Reset time to start new delay
void Repeater::ResetTime()
{
    this->time_from_repeat_ms = millis();
}