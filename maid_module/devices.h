#pragma once

#include <Arduino.h>
#include "models.h"
#include "typedefs.h"


// * Provides work with bluetooth connection
class BluetoothAdapter
{
public:
    static void TransferMessage(Message &msg);
};

// * Provides work with speed calculation
class Speedometer
{
private:
    uint impulse_counter;
public:
    Speedometer(int counter);
    float CalculateSpeed(byte time_spent_sec, Wheel &wheel);
    void CountImpulse();
    void ResetCounter();
};

// * Provides work with voltage calculation
class Voltmeter
{
private:
    byte R1_kohm;
    byte R2_kohm;
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
    static void ManipulateSingleSignal(Side side, Interaction mode);
};