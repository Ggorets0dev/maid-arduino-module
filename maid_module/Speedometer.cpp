#include "devices.h"

// * Creatings speedometer with initial impusle count
Speedometer::Speedometer()
{
    this->impulse_counter = 0;
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

// * Get the number of counted pulses
int Speedometer::GetImpulseCount() const
{
    return this->impulse_counter;
}

// * Calculate speed with spoke count and time elapsed, parameters of wheel needed
float Speedometer::CalculateSpeed(float time_spent_sec, const Wheel &wheel) const
{
    if (impulse_counter != 0) 
    {
        const float speed = (static_cast<float>(this->impulse_counter) / static_cast<float>(wheel.count_of_spokes) * static_cast<float>(wheel.wheel_circumference_mm) / pow(10, 6)) * (pow(60, 2) / time_spent_sec);
        return speed;
    }
    else 
        return 0.0f;
}
