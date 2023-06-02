#pragma once

#include <Arduino.h>
#include "pinout.h"
#include "tools.h"
#include "typedefs.h"

// ! Declaring classes from other header files in advance
class MessageAnalyzer;

// * Node of linked list with sensor readings
class Node
{
public:
    static byte node_cnt;
    static constexpr byte max_node_cnt = 10; // ! Data may not be saved if memory consumption is too high
    byte impulse_cnt;
    uint analog_voltage;
    ulong time;
    Node* next;

    Node(byte impulse_cnt, uint analog_voltage, ulong millis) : impulse_cnt(impulse_cnt), analog_voltage(analog_voltage), time(millis), next(nullptr) {};
    static Node* CreateHead(byte impulse_cnt, uint analog_voltage, ulong millis);
    static void Insert(Node* head, byte impulse_cnt, uint analog_voltage, ulong millis);
    static void DeleteAll(Node* head);
};

// * Message which can be sent/recieved using bluetooth
class Message
{
public:
    static const byte maximal_message_length = 25;
    char prefix;
    byte code;
    String data;

    static bool IsValid(const Message &msg);

    Message(char prefix, byte code, String data) : prefix(prefix), code(code), data(data) {};
    Message(String msg);
    Message(float speed, float voltage);
    Message();
    String ToString() const;
};

// * Wheel used in motorcycle
struct Wheel
{
    byte count_of_spokes;
    uint wheel_circumference_mm;

    Wheel(byte spokes_cnt, uint wheel_circ_mm);
};

// * Measured speed and voltages and their initial variants
struct Reading
{
    byte impulse_cnt = 0;
    uint analog_voltage = 0;
    float speed_kmh = 0.0f;
    float voltage_v = 0.0f;
};
