# pragma once
#include <Arduino.h>

# define READINGS '#'
# define COMMAND '$'

namespace Models
{
    // * Motorcycle wheel model with the appropriate indicators needed to calculate speed and distance
    class Wheel
    {
    public:
        unsigned short count_of_spokes;
        unsigned short wheel_circumference_mm;
        Wheel(unsigned short spokes, unsigned short size_mm);
    };

    // * A class with all the functions needed for data processing
    class Instruments
    {
    public:
        String CreateTransferMessage(char prefix, String msg);
        String CreateTransferMessage(byte speed, byte voltage, char prefix='#');
    };
}
