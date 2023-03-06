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
    static uint node_cnt;
    static constexpr uint max_node_cnt = 15; // ! Memory consumption is directly related to this amount
    int impulse_cnt;
    int analog_voltage;
    ulong time;
    Node* next;

    Node(int impulse_cnt, int analog_voltage, ulong millis);
    static Node* CreateHead(int impulse_cnt, int analog_voltage, ulong millis);
    static void Insert(Node* head, int impulse_cnt, int analog_voltage, ulong millis);
    static void DeleteAll(Node* head);
};

// * Message which can be sent/recieved using bluetooth
class Message
{
public:
    static const uint maximal_message_length = 25;
    char prefix;
    int code;
    String data;

    Message(String msg);
    Message(char prefix, int code, String data);
    Message(float speed, float voltage);
    Message();
    String ToString() const;
    static bool IsValid(const Message &msg);
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
    int impulse_cnt = 0;
    int analog_voltage = 0;
    float speed_kmh = 0.0f;
    float voltage_v = 0.0f;
};
