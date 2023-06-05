#include "devices.h"

// * Creating Signal class with it's blink settings
Signal::Signal(byte led_pin, float delay_sec, bool enabled, float reaction_interval_sec)
{
    this->led_pin = led_pin;
    this->is_shining = false;
    this->ChangeStateTimer = Timer(delay_sec);
    this->reaction_interval_sec = reaction_interval_sec;
    
    if (!enabled)
        this->ChangeStateTimer.Disable();
}

// * Checks if time is in device's reaction interval
bool Signal::IsInReactionInterval(ulong time) const
{
    const float reaction_interval_ms = this->reaction_interval_sec * 1000.0f;
    const float time_difference_ms = abs(static_cast<float>(millis()) - reaction_interval_ms);
    const bool is_within_range = static_cast<float>(time) > time_difference_ms;

    return is_within_range;
}

// * Blinks using timer inside the class
void Signal::TryBlink()
{
    if (ChangeStateTimer.IsPassed() && ChangeStateTimer.IsEnabled())
    {
        if (is_shining)
            digitalWrite(led_pin, LOW);
        else
            digitalWrite(led_pin, HIGH);
        
        this->is_shining = !this->is_shining;
        ChangeStateTimer.ResetTime();
    }

    else if (!ChangeStateTimer.IsEnabled() && this->is_shining)
    {
        digitalWrite(led_pin, LOW);
        this->is_shining = false;
    }
}

// * Transition to infinite blink to indicate a critical situation
void Signal::BlinkForever(float delay_sec) const
{
	  const float delay_ms = delay_sec * 1000.0f;
    while (true)
    {
        digitalWrite(led_pin, HIGH);
        delay(delay_ms);
        digitalWrite(led_pin, LOW);
        delay(delay_ms);
    }
}
