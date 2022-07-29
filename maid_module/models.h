# pragma once
#include <Arduino.h>

# define READINGS '#'
# define COMMAND '$'

namespace Models
{
    // * Struct with the appropriate indicators needed to calculate speed and distance
    struct Wheel
    {
        byte count_of_spokes;
        unsigned short wheel_circumference_mm;
        Wheel(byte spokes, unsigned short size_mm);
    };

    // * Provides work with bluetooth connection
    class BluetoothAdapter
    {
    public:
        String CreateTransferMessage(char prefix, String msg);
        String CreateTransferMessage(byte speed, byte voltage, char prefix='#');
    };

    // * Provides work with speed calculation
    class Speedometer
    {
    public:
        float CalculateSpeed(byte time_spent, byte num_of_impulses, Wheel wheel);
    };

    // * Provides work with voltage calculation
    class Voltmeter
    {
    private:
        byte R1_kohm; // Used in voltage divider
        byte R2_kohm; // Used in voltage divider
    public:
        Voltmeter(byte r1_kohm, byte r2_kohm);
        float CalculateVoltage(unsigned short analog_read_result);
    };

    // * Provides work with all available signals
    class Signaler
    {
    public:
        void TurnOnSignal(byte pin);
        void TurnOffSignal(byte pin);
    };
}
