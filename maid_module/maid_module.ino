/*
  Project: MaidModule
  Repository: maid-arduino-module
  Developer: Ggorets0dev
  Version: 0.7.4(E7)
  GitHub page: https://github.com/Ggorets0dev/maid-arduino-module
*/

#define __MODULE_VERSION__ "0.7.4(E7)"

#include <Arduino.h>
#include "PinChangeInterrupt.h"
#include "pinout.h"
#include "typedefs.h"
#include "models.h" 
#include "devices.h"
#include "tools.h"

bool IsInitialized = false;

// * Variables-buffers for temporary storage
float measured_speed;
float measured_voltage;
Message msg_temp;
Node* head;

// * Technical vars required for the application
Timer SendReadingsTimer(2.0f);
Timer SaveReadingsTimer(0.5f);
const uint BAUD = 9600; 

// * Settings of linked list with sensor readings
uint Node::max_node_cnt = 10;    // ! Memory consumption is directly related to this amount
uint Node::node_cnt = 0;         // ! Must be 0 for correct start of sketch

// * Initialization of all classes-devices required for work
Wheel FrontWheel(8, 2070);
Voltmeter VoltageSensor(10, 100);
Speedometer SendSpeedSensor(0);
Speedometer SaveSpeedSensor(0);
Signaling Signaler(Signaling::Mode::Disabled, Signaling::Mode::Disabled);
Logging Logger("logs.log", "blocks.txt");

Sd2Card card;
SdVolume volume;
SdFile root;

// * Creating interrupt handlers for all available interrupts
void HandleSpeedometer(void) 
{ 
    SendSpeedSensor.CountImpulse();
    SaveSpeedSensor.CountImpulse();
}
void HandleLeftTurnOn(void) { Signaler.EnableTurn(Signaling::Side::Left); }
void HandleLeftTurnOff(void) { Signaler.DisableTurn(Signaling::Side::Left); }
void HandleRightTurnOn(void) { Signaler.EnableTurn(Signaling::Side::Right); }
void HandleRightTurnOff(void) { Signaler.DisableTurn(Signaling::Side::Right); }


void setup() 
{
    Serial.begin(BAUD);
    SD.begin(MEMORY_PIN);
    
    while (!Serial) {
      ; // * Wait for serial port to connect. Needed for native USB port only
    }

    if (!Memory::InitROM(card, volume, root))
        Serial.println("Failed to load memory functions");

    else if (Memory::GetFreeROM(volume) < Memory::minimal_free_rom_size)
        Serial.println("No free space on card");
    
    Serial.println(Memory::GetFreeROM(volume));

    pinMode(SPEEDOMETER_PIN, INPUT_PULLUP);
    // pinMode(RIGHT_TURN_BUTTON_PIN, INPUT_PULLUP);
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
    if (SaveReadingsTimer.IsPassed() && SaveReadingsTimer.IsEnabled() && IsInitialized)
    {
        measured_speed = SaveSpeedSensor.CalculateSpeed(SaveReadingsTimer.GetRepeatTime(), FrontWheel);
        measured_voltage = VoltageSensor.CalculateVoltage(analogRead(VOLTMETER_PIN));

        if (Node::node_cnt == 0)
            head = Node::CreateHead(measured_speed, measured_voltage);
        
        else
            Node::Insert(head, measured_speed, measured_voltage);

        if (Node::node_cnt >= Node::max_node_cnt || Memory::GetFreeRAM() < Memory::minimal_free_ram_size)
        {
            Serial.println("Got all!");
            Logger.WriteBlocks(head);
            Node::DeleteAll(head);
        }

        Serial.println(Memory::GetFreeRAM());
        SaveSpeedSensor.ResetCounter(); 
        SaveReadingsTimer.ResetTime();
    }

    if (SendReadingsTimer.IsPassed() && SendReadingsTimer.IsEnabled() && IsInitialized) 
    {
        measured_speed = SendSpeedSensor.CalculateSpeed(SendReadingsTimer.GetRepeatTime(), FrontWheel);
        measured_voltage = VoltageSensor.CalculateVoltage(analogRead(VOLTMETER_PIN));

        msg_temp = Message(measured_speed, measured_voltage);

        BluetoothAdapter::TransferMessage(msg_temp);
        
        SendSpeedSensor.ResetCounter(); 
        SendReadingsTimer.ResetTime();
    }

    if (Serial.available())
    {
        msg_temp = BluetoothAdapter::RecieveMessage();

        //Serial.println("Got: " + msg_temp.ToString());

        if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::InitializationDone))
            IsInitialized = true;
  
        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::GetNowDate))
        {            
            if (Logger.TrySetDate(msg_temp.GetData()))
            {
                Logger.WriteHeader(FrontWheel, SaveReadingsTimer);
                msg_temp = Message(MessageAnalyzer::MessagePrefixes::Response, MessageAnalyzer::MessageCodes::SendModuleVersion, __MODULE_VERSION__);
                BluetoothAdapter::TransferMessage(msg_temp);
            }
        }

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::StartSensorReadings))
            SendReadingsTimer.Enable();
        
        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::StopSensorReadings))
            SendReadingsTimer.Disable();
    }
}
