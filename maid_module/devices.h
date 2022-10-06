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
    Speedometer(uint impulse_cnt);
    float CalculateSpeed(float time_spent_sec, Wheel &wheel);
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
    static const uint minimal_reading_value = 0;
    Voltmeter(byte r1_kohm, byte r2_kohm);
    float CalculateVoltage(int analog_read_result);
};

// * Provides work with left and right back signals
class Signaler
{
private:
    bool IsLeftTurnEnabled;
    bool IsRightTurnEnabled;
	uint LeftTurnMillis;
	uint RightTurnMillis;
public:
    enum Mode
    {
        Enabled = true,
        Disabled = false
    };

    enum Side
    {
        Right = RIGHT_TURN_LAMP_PIN,
        Left = LEFT_TURN_BUTTON_PIN
    };
    
    Signaler(Mode left_turn_mode, Mode right_turn_mode);
    bool IsEnabled(Side side);
    void EnableTurn(Side side);
    void DisableTurn(Side side);
};
