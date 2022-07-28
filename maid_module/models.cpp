#include "models.h"
#include <Arduino.h>

namespace Models
{
    Wheel::Wheel(unsigned short spokes, unsigned short size_mm)
    {
        count_of_spokes = spokes;
        wheel_circumference_mm = size_mm;
    }

    String Instruments::CreateTransferMessage(char prefix, String msg)
    {
        String command = " !" + msg + ";";
        command[1] = prefix;
        return command;
    }

    String Instruments::CreateTransferMessage(byte speed, byte voltage, char prefix=READINGS)
    {
        String command = " !" + String(speed) + "_" + String(voltage) + ";";
        command[1] = prefix;
        return command;
    }
}