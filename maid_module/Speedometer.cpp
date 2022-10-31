#include "devices.h"

// * Creatings speedometer with initial impusle count
Speedometer::Speedometer(uint impulse_cnt)
{
    this->impulse_counter = impulse_cnt;
}

// * Increase the counter reading after passing the spoke of wheel
void Speedometer::CountImpulse() 
{ 
    this->impulse_counter++;
}

// * Reseting spoke/impulse counter
void Speedometer::ResetCounter() 
{ 
    this->impulse_counter = 0; 
}

int Speedometer::GetImpulseCount() const
{
    return this->impulse_counter;
}

// * Calculate speed with spoke count and time elapsed, parameters of wheel needed
float Speedometer::CalculateSpeed(float time_spent_sec, Wheel &wheel) const
{
    if (impulse_counter != 0) 
    {
        float speed = ((float)impulse_counter / (float)wheel.count_of_spokes * (float)wheel.wheel_circumference_mm / 1000000.0f) * (60.0f * 60.0f / time_spent_sec);
        return speed;
    }
    else 
        return 0.0f;
}
