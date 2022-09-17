#include "devices.h"

Voltmeter::Voltmeter(byte r1_kohm, byte r2_kohm) 
{ 
    this->R1_kohm = r1_kohm; 
    this->R2_kohm = r2_kohm; 
}

// * Calculate voltage using readings from analog arduino port
float Voltmeter::CalculateVoltage(int analog_read_result)
{
    float vout = ((float)analog_read_result * 5.0) / 1024.0;
    float vin = vout / ((float)R2_kohm / ((float)R1_kohm + (float)R2_kohm));
    return vin;
}
