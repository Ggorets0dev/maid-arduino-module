#include "tools.h"

// * Creating a Timer class with delay
Timer::Timer(float repetition_time_sec)
{
    this->time_from_repeat_ms = 0;
    this->repetition_time_sec = repetition_time_sec;
}

// * Getting delay from class for calculations
float Timer::GetRepeatTime()
{
    return this->repetition_time_sec;
}

// * Checking if delay is ended
bool Timer::IsPassed()
{
    return ((float)millis() - (float)time_from_repeat_ms) / 1000.0 >= repetition_time_sec;
}

// * Reset time to start new delay
void Timer::ResetTime()
{
    this->time_from_repeat_ms = millis();
}