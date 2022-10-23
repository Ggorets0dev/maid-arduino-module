#include "devices.h"

// * Creating Signal class with it's blink settings
Signal::Signal(uint led_pin, float delay_sec, bool enabled, float reaction_interval_sec=1.0f)
{
    this->led_pin = led_pin;
    this->is_shining = false;
    this->ChangeStateTimer = Timer(delay_sec);
    this->reaction_interval_sec = reaction_interval_sec;
    
    if (!enabled)
        this->ChangeStateTimer.Disable();
}

// * Checks if time is in device's reaction interval
bool Signal::IsInReactionInterval(ulong time)
{
    return (float)time > (abs((float)millis() - reaction_interval_sec) * 1000.0f);
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
        
        is_shining = !is_shining;
        ChangeStateTimer.ResetTime();
    }

    else if (!ChangeStateTimer.IsEnabled() && is_shining)
    {
        digitalWrite(led_pin, LOW);
        is_shining = false;
    }
}

void Signal::BlinkForever(float multiplier=1.0f)
{
	float delay_ms = ChangeStateTimer.GetRepeatTime() * multiplier * 1000.0f;
    while (true)
    {
        digitalWrite(led_pin, HIGH);
        delay(delay_ms);
        digitalWrite(led_pin, LOW);
        delay(delay_ms);
    }
}
