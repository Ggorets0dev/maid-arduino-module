#pragma once
#include "pinout.h"
#include "transfer_prefixes.h"
#include <Arduino.h>

typedef unsigned short ushort;
typedef unsigned long ulong;

namespace Models
{
    // * Wheel used in motorcycle
    class Wheel
    {
    private:
        byte count_of_spokes;
        ushort wheel_circumference_mm;
    public:
        Wheel(byte spokes, ushort size_mm);
        byte GetSpokesCount();
        ushort GetWheelCircumference();
    };

    // * Provides work with bluetooth connection
    class BluetoothAdapter
    {
    public:
        void TransferMessage(String msg, char prefix=READINGS_PREFIX);
        void TransferRecords(float speed, float voltage, char prefix=READINGS_PREFIX);
    };

    // * Provides work with speed calculation
    class Speedometer
    {
    private:
        ushort impulse_counter;
    public:
        float CalculateSpeed(byte time_spent_sec, Wheel wheel);
        void CountImpulse();
        void ResetCounter();
    };

    // * Provides work with voltage calculation
    class Voltmeter
    {
    private:
        byte R1_kohm; // Used in voltage divider
        byte R2_kohm; // Used in voltage divider
    public:
        Voltmeter(byte r1_kohm, byte r2_kohm);
        float CalculateVoltage(int analog_read_result);
    };

    // * Provides work with all available signals
    class Signaler
    {
    public:
        enum Side
        {
            Right = RIGHT_TURN_LAMP_PIN,
            Left = LEFT_TURN_BUTTON_PIN
        };
        enum Interaction
        {
            On = HIGH,
            Off = LOW
        };
        void ManipulateSingleSignal(Side side, Interaction mode);
    };
}
