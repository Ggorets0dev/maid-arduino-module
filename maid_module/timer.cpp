#include "tools.h"

Timer::Timer(float repetition_time_sec)
{
    this->time_from_repeat_ms = 0;
    this->repetition_time_sec = repetition_time_sec;
    this->is_enabled = true;
}

Timer::Timer()
{
    this->time_from_repeat_ms = 0;
    this->repetition_time_sec = 2;
    this->is_enabled = true;
}

// * Getting delay from class for calculations
float Timer::GetRepeatTime() const
{
    return this->repetition_time_sec;
}

// * Checking if delay is ended
bool Timer::IsPassed() const
{
    return (static_cast<float>(millis()) - static_cast<float>(this->time_from_repeat_ms)) / 1000.0f >= this->repetition_time_sec;
}

// * Check if time counting is enabled
bool Timer::IsEnabled() const
{
    return this->is_enabled;
}

// * Enable time counting
void Timer::Enable()
{
    this->is_enabled = true;
    ResetTime();
}

// * Disable time counting
void Timer::Disable()
{
    this->is_enabled = false;
}

// * Reset time to start new delay
void Timer::ResetTime()
{
    this->time_from_repeat_ms = millis();
}