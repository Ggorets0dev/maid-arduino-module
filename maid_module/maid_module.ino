/*
  Project: MaidModule
  Repository: maid-arduino-module
  Developer: Ggorets0dev
  Version: 0.7.3
  GitHub page: https://github.com/Ggorets0dev/maid-arduino-module
*/

#define __MODULE_VERSION__ "0.7.3"


#include <Arduino.h>
#include "PinChangeInterrupt.h"
#include "pinout.h"
#include "typedefs.h"
#include "models.h" 
#include "devices.h"
#include "tools.h"

// * Variables-buffers for temporary storage
float measured_speed;
float measured_voltage;
ulong time_transfer_readings_ms = 0;
ulong time_save_readings_ms = 0;
byte msg_byte_buffer[Message::maximal_message_length];
bool SendReadings = true;
Message msg_temp;
Node* head;

// * Technical constants required for the application
Repeater SendReadingsRepeater(2.0);
Repeater SaveReadingsRepeater(0.5);
const uint BAUD = 9600; 

// * Settings of linked list with sensor readings
uint Node::max_node_cnt = 10;    // ! Memory consumption is directly related to this amount
uint Node::node_cnt = 0;         // ! Must be 0 for correct start of sketch

// * Initialization of all classes-devices required for work
Wheel FrontWheel(8, 2070);
Voltmeter VoltageSensor(10, 100);
Speedometer SendReadingsSensor(0);
Speedometer SaveReadingsSensor(0);
Signaler Signal(false, false);


// * Creating interrupt handlers for all available interrupts
void HandleSpeedometer(void) 
{ 
    SendReadingsSensor.CountImpulse();
    SaveReadingsSensor.CountImpulse();
}
void HandleLeftTurnOn(void) { Signal.EnableTurn(Signaler::Side::Left); }
void HandleLeftTurnOff(void) { Signal.DisableTurn(Signaler::Side::Left); }
void HandleRightTurnOn(void) { Signal.EnableTurn(Signaler::Side::Right); }
void HandleRightTurnOff(void) { Signal.DisableTurn(Signaler::Side::Right); }


void setup() 
{
    Serial.begin(BAUD);

    // randomSeed(analogRead(0)); // For testing readings transfering

    pinMode(SPEEDOMETER_PIN, INPUT_PULLUP);
    pinMode(RIGHT_TURN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LEFT_TURN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LEFT_TURN_LAMP_PIN, OUTPUT);
    pinMode(RIGHT_TURN_LAMP_PIN, OUTPUT);
    
    attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), HandleLeftTurnOn, RISING);
    attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), HandleRightTurnOn, RISING);
    attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), HandleRightTurnOff, FALLING);
    attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), HandleLeftTurnOff, FALLING);
    attachInterrupt(0, HandleSpeedometer, FALLING);
}

void loop() 
{
    if (SaveReadingsRepeater.IsPassed())
    {
        measured_speed = SaveReadingsSensor.CalculateSpeed(SaveReadingsRepeater.GetRepeatTime(), FrontWheel);
        measured_voltage = VoltageSensor.CalculateVoltage(analogRead(VOLTMETER_PIN));

        if (Node::node_cnt == 0)
            head = Node::CreateHead(measured_speed, measured_voltage);
        
        else
            Node::InsertNode(head, measured_speed, measured_voltage);

        if (Node::node_cnt >= Node::max_node_cnt)
        {
            Serial.println("Got all!");
            Node::DeleteAllNodes(head);
        }

        SaveReadingsSensor.ResetCounter(); 
        SaveReadingsRepeater.ResetTime();
    }

    if (SendReadingsRepeater.IsPassed() && SendReadings) 
    {
        measured_speed = SendReadingsSensor.CalculateSpeed(SendReadingsRepeater.GetRepeatTime(), FrontWheel);
        measured_voltage = VoltageSensor.CalculateVoltage(analogRead(VOLTMETER_PIN));

        msg_temp =  Message(measured_speed, measured_voltage);

        BluetoothAdapter::TransferMessage(msg_temp);
        
        SendReadingsSensor.ResetCounter(); 
        SendReadingsRepeater.ResetTime();
    }

    if (Serial.available())
    {
        Serial.readBytes(msg_byte_buffer, sizeof(msg_byte_buffer));
        msg_temp = Message(String((char*)msg_byte_buffer));
        memset(msg_byte_buffer, 0, sizeof(msg_byte_buffer));

        // Serial.println(msg_temp.ToString());

        if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::GetModuleVersion))
        {
            msg_temp = Message(MessageAnalyzer::MessagePrefixes::Response, MessageAnalyzer::MessageCodes::SendModuleVersion, __MODULE_VERSION__);
            BluetoothAdapter::TransferMessage(msg_temp);
        }
        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::StartSensorReadings))
            SendReadings = true;
        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::StopSensorReadings))
            SendReadings = false;
    }
}
