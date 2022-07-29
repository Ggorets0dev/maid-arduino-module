#include "models.h"
#include <Arduino.h>

namespace Models
{
    Wheel::Wheel(byte spokes, unsigned short size_mm) { count_of_spokes = spokes; wheel_circumference_mm = size_mm; }

    String BluetoothAdapter::CreateTransferMessage(char prefix, String msg)
    {
        String command = " !" + msg + ";";
        command[1] = prefix;
        return command;
    }
    String BluetoothAdapter::CreateTransferMessage(byte speed, byte voltage, char prefix=READINGS)
    {
        String command = " !" + String(speed) + "_" + String(voltage) + ";";
        command[1] = prefix;
        return command;
    }

    Voltmeter::Voltmeter(byte r1_kohm, byte r2_kohm) { R1_kohm = r1_kohm; R2_kohm = r2_kohm; }

    void Signaler::TurnOnSignal(byte pin) { digitalWrite(pin, HIGH); }
    void Signaler::TurnOffSignal(byte pin) { digitalWrite(pin, LOW); }
}