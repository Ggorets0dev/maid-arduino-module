#include "devices.h"

// * Creating voltmeter class with initial R values
Voltmeter::Voltmeter(byte r1_kohm, byte r2_kohm) 
{ 
    this->R1_kohm = r1_kohm; 
    this->R2_kohm = r2_kohm; 
}

// * Calculate voltage using readings from analog arduino port
float Voltmeter::CalculateVoltage(int analog_read_result)
{
    float vout = ((float)analog_read_result * 5.0f) / 1024.0f;
    float vin = vout / ((float)R2_kohm / ((float)R1_kohm + (float)R2_kohm));
    
    if (vin >= (float)minimal_reading_value)
        return vin;
    else
        return 0.0f;
}
