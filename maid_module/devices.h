#pragma once

#include <Arduino.h>
#include <AltSoftSerial.h>
#include <SPI.h>
#include <SD.h>
#include "models.h"
#include "typedefs.h"


// * Provides work with bluetooth connection
class BluetoothAdapter
{
public:
    static void TransferMessage(const Message &msg, AltSoftSerial &BtSerial);
    static Message RecieveMessage(AltSoftSerial &BtSerial);
};

// * Provides work with speed calculation
class Speedometer
{
private:
    uint impulse_counter;
    
public:
    Speedometer() : impulse_counter(0) {};
    float CalculateSpeed(float time_spent_sec, const Wheel &wheel) const;
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
    static constexpr float correction_factor = 1.0347f;
    
    Voltmeter(byte max_voltage) : max_voltage(max_voltage) {};
    byte GetMaxVoltage() const;
    float CalculateVoltage(int analog_read_result) const;
};

// * Provides work with leds and lamps
class Signal
{
private:
    byte led_pin;
    bool is_shining;
    float reaction_interval_sec;
    
public:
    Timer ChangeStateTimer; 
    
    Signal(byte led_pin, float delay_sec, bool enabled, float reaction_interval_sec);
    bool IsInReactionInterval(ulong time) const;
    void TryBlink();
    void BlinkForever(float multiplier) const;
};

// * Provides work with files (sd-card)
class DataSaver
{
private:
    static const uint date_time_length = 19; // dd.MM.yyyy-HH:mm:ss
    String date_time;
    String readings_filename;
    ulong last_write_time;
    
public:
    DataSaver(String readings_filename);
    ulong GetLastWriteTime() const;
    bool TrySetDateTime(String date_time);
    void WriteNodes(Node* head);
    void WriteHeader(const Voltmeter &voltmeter, const Wheel &wheel, const Timer &save_readings_timer); 
};
