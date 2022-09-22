#include "devices.h"

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
    if (impulse_counter == 0) 
        return 0.0;
    else 
        return ((float)impulse_counter / (float)wheel.GetSpokesCount() * (float)wheel.GetWheelCircumference() / 1000000) * (60 * 60 / time_spent_sec);
}

// * Calculates the average speed over the elapsed time between nodes
float CalculateAverageSpeed(Node* head)
{
    float speed_average = 0.0;
    float speed_sum = 0.0;
    float time_sum = 0.0;
    return speed_average;
}