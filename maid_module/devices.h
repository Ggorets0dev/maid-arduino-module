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
    float CalculateSpeed(byte time_spent_sec, Wheel &wheel);
    float CalculateAverageSpeed(Node* head);
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

// * Provides work with left and right back signals
class Signaler
{
private:
    bool IsLeftTurnEnabled;
    bool IsRightTurnEnabled;
	uint LeftTurnMillis;
	uint RightTurnMillis;
public:
    enum Side
    {
        Right = RIGHT_TURN_LAMP_PIN,
        Left = LEFT_TURN_BUTTON_PIN
    };
    
    Signaler(bool left_turn_mode, bool right_turn_mode);
    bool IsEnabled(Side side);
    void EnableTurn(Side side);
    void DisableTurn(Side side);
};
