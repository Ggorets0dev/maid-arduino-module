#include "devices.h"

// * Get max input voltage on pin
byte Voltmeter::GetMaxVoltage() const
{
    return this->max_voltage;
}

// * Calculate voltage using readings from analog Arduino port
float Voltmeter::CalculateVoltage(uint analog_voltage) const
{
    const float vin = static_cast<float>(analog_voltage) * static_cast<float>(this->max_voltage) / 1023.0f * Voltmeter::correction_factor;

    if (vin >= Voltmeter::minimal_reading_value)
        return vin;
    else
        return 0.0f;
}
