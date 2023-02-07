#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "models.h"
#include "typedefs.h"


// * Provides work with bluetooth connection
class BluetoothAdapter
{
public:
    static void TransferMessage(Message &msg);
    static Message RecieveMessage();
};

// * Provides work with speed calculation
class Speedometer
{
private:
    uint impulse_counter;
public:
    Speedometer(uint impulse_cnt);
    float CalculateSpeed(float time_spent_sec, Wheel &wheel) const;
    int GetImpulseCount() const;
    void CountImpulse();
    void ResetCounter();
};

// * Provides work with voltage calculation
class Voltmeter
{
private:
    byte max_voltage;
public:
    static constexpr float minimal_reading_value = 0.0f;
    
    Voltmeter(byte max_voltage);
    byte GetMaxVoltage() const;
    float CalculateVoltage(int analog_read_result) const;
};

// * Provides work with leds and lamps
class Signal
{
private:
    uint led_pin;
    bool is_shining;
    float reaction_interval_sec;
public:
    Timer ChangeStateTimer; 
    
    Signal(uint led_pin, float delay_sec, bool enabled, float reaction_interval_sec=1.0f);
    bool IsInReactionInterval(ulong time) const;
    void TryBlink();
    void BlinkForever(float multiplier=1.0f) const;
};

// * Provides work with files (sd-card)
class Logging
{
private:
    static constexpr uint date_time_length = 19; // dd.MM.yyyy-HH:mm:ss
    String date_time;
    String readings_filename;
    ulong last_write_time;
public:
    Logging(String readings_filename);
    ulong GetLastWriteTime() const;
    bool TrySetDateTime(String date_time);
    void WriteNodes(Node* head);
    void WriteHeader(Voltmeter &voltmeter, Wheel &wheel, Timer &save_readings_timer); 
};
