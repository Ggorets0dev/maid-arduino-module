#pragma once
#include "pinout.h"
#include <Arduino.h>


namespace Models
{
    // * Wheel used in motorcycle
    class Wheel
    {
    private:
        byte count_of_spokes;
        unsigned short wheel_circumference_mm;
    public:
        Wheel(byte spokes, unsigned short size_mm);
        byte GetSpokesCount();
        unsigned short GetWheelCircumference();
    };

    // * Provides work with bluetooth connection
    class BluetoothAdapter
    {
    public:
        void TransferMessage(char prefix, String msg);
        void TransferRecords(float speed, float voltage, char prefix='#');
    };

    // * Provides work with speed calculation
    class Speedometer
    {
    private:
        unsigned short impulse_counter;
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
        void TurnOnRightSignal();
        void TurnOffRightSignal();
        void TurnOnLeftSignal();
        void TurnOffLeftSignal();
    };
}
