#include "devices.h"

// * Creatings speedometer with initial impusle count
Speedometer::Speedometer(uint impulse_cnt)
{
    this->impulse_counter = impulse_cnt;
}

// * Increase the counter reading after passing the spoke of wheel
void Speedometer::CountImpulse() 
{ 
    impulse_counter++; 
}

// * Reseting spoke/impulse counter
void Speedometer::ResetCounter() 
{ 
    impulse_counter = 0; 
}

// * Calculate speed with spoke count and time elapsed, parameters of wheel needed
float Speedometer::CalculateSpeed(float time_spent_sec, Wheel &wheel)
{
    float speed = ((float)impulse_counter / (float)wheel.GetSpokesCount() * (float)wheel.GetWheelCircumference() / 1000000.0f) * (60.0f * 60.0f / time_spent_sec);
    
    if (impulse_counter != 0) 
        return speed;
    else 
        return 0.0f;
}