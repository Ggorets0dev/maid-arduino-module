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

	Node(int impulse_cnt, int analog_voltage);
	static Node* CreateHead(int impulse_cnt, int analog_voltage);
	static void Insert(Node* head, int impulse_cnt, int analog_voltage);
	static void DeleteAll(Node* head);
};

// * Message which can be sent/recieved using bluetooth
class Message
{
private:
    char prefix;
    int code;
    String data;
public:
    static constexpr uint maximal_message_length = 20;
    
    Message(String msg);
    Message(char prefix, int code, String data);
    Message(float speed, float voltage);
    Message();
    char GetPrefix();
    int GetCode();
    String GetData();
    String ToString();
};

// * Wheel used in motorcycle
struct Wheel
{
    byte count_of_spokes;
    uint wheel_circumference_mm;

    Wheel(byte spokes_cnt, uint wheel_circ_mm);
};

struct Reading
{
    int impulse_cnt = 0;
    int analog_voltage = 0;
    float speed_kmh = 0.0f;
    float voltage_v = 0.0f;
};
