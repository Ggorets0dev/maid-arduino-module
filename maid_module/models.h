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
    static uint max_node_cnt;
	float speed_kmh;
	float voltage_v;
	class Node* next;

	Node(float speed, float voltage);
	static class Node* CreateHead(float speed, float voltage);
	static void InsertNode(class Node* head, float speed, float voltage);
	static void DeleteAllNodes(class Node* head);
};

// * Message which can be sent/recieved using bluetooth
class Message
{
private:
    char prefix;
    int code;
    String data;
public:
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
class Wheel
{
private:
    byte count_of_spokes;
    uint wheel_circumference_mm;
public:
    Wheel(byte spokes, uint size_mm);
    byte GetSpokesCount();
    uint GetWheelCircumference();
};
