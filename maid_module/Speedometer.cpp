#include "devices.h"

void Speedometer::CountImpulse() 
{ 
    impulse_counter++; 
}

void Speedometer::ResetCounter() 
{ 
    impulse_counter = 0; 
}

float Speedometer::CalculateSpeed(byte time_spent_sec, Wheel wheel)
{
    if (impulse_counter == 0) return 0.0;
    else 
    {
        float speed_kmh = ((float)impulse_counter / (float)wheel.GetSpokesCount() * (float)wheel.GetWheelCircumference() / 1000000) * (60 * 60 / (float)time_spent_sec);
        return speed_kmh;
    }
}