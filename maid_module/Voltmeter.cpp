#include "devices.h"

// * Creating Voltmeter class with initial voltage value
Voltmeter::Voltmeter(byte max_voltage) 
{ 
    this->max_voltage = max_voltage;
}

// * Get max input voltage on pin
byte Voltmeter::GetMaxVoltage() const
{
    return this->max_voltage;
}

// * Calculate voltage using readings from analog Arduino port
float Voltmeter::CalculateVoltage(int analog_read_result) const
{
    const float vin = (float)analog_read_result * (float)this->max_voltage / 1023.0f;

    if (vin >= this->minimal_reading_value)
        return vin;
    else
        return 0.0f;
}
