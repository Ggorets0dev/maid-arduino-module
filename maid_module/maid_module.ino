/*
  Project: MaidModule
  Repository: maid-arduino-module
  Developer: Ggorets0dev
  Version: 0.13.1
  GitHub page: https://github.com/Ggorets0dev/maid-arduino-module
*/

#define __MODULE_VERSION__ "0.13.1"

#include <Arduino.h>
#include "PinChangeInterrupt.h"
#include "pinout.h"
#include "typedefs.h"
#include "models.h" 
#include "devices.h"
#include "tools.h"

// NOTE - Variables-buffers for temporary storage
Reading last_reading;
Message msg_temp;

// NOTE - Technical vars required for the application
const uint BAUD = 9600; 
bool IsInitialized = false;
bool IsAlert = false;
Timer SendReadingsTimer(2.0f);
Timer SaveReadingsTimer(0.5f);
MillisTracker millis_passed;

// NOTE - Settings of linked list with sensor readings
uint Node::node_cnt = 0; // NOTE - Starting the node count from zero
Node* head = NULL;

// NOTE - Initialization of all classes-devices required for work
Wheel FrontWheel(8, 2070);
Voltmeter VoltageSensor(60);
Speedometer SpeedSensor(0);
Signal SdCardSaving(ROM_SAVE_SIGNAL_PIN, 0.20f, false, 2.0f);
Signal ErrorOccuring(ERROR_SIGNAL_PIN, 0.5f, false);
Signal LeftTurning(LEFT_TURN_LAMP_PIN, 1.0f, false);
Signal RightTurning(RIGHT_TURN_LAMP_PIN, 1.0f, false);
Logging Logger("data.txt");

Sd2Card card;
SdVolume volume;
SdFile root;

// SECTION - Interruption handlers
void HandleLeftTurnChangeState(void)
{             
    if (LeftTurning.ChangeStateTimer.IsEnabled())
        LeftTurning.ChangeStateTimer.Disable(); 
    else
        LeftTurning.ChangeStateTimer.Enable();
}
void HandleRightTurnChangeState(void)
{             
    if (RightTurning.ChangeStateTimer.IsEnabled())
        RightTurning.ChangeStateTimer.Disable(); 
    else
        RightTurning.ChangeStateTimer.Enable();
}
void HandleSpeedometer(void) 
{ 
    SpeedSensor.CountImpulse(); 
}
// !SECTION

// SECTION - Working with turn signals via codes from control devices
void HandleLeftTurnOff(void)
{             
    if (LeftTurning.ChangeStateTimer.IsEnabled())
        LeftTurning.ChangeStateTimer.Disable(); 
}
void HandleRightTurnOff(void)
{
    if (RightTurning.ChangeStateTimer.IsEnabled())
        RightTurning.ChangeStateTimer.Disable();
}
void HandleLeftTurnOn(void) 
{             
    if (!LeftTurning.ChangeStateTimer.IsEnabled())
        LeftTurning.ChangeStateTimer.Enable();
    
    HandleRightTurnOff();
}
void HandleRightTurnOn(void)
{
    if (!RightTurning.ChangeStateTimer.IsEnabled())
        RightTurning.ChangeStateTimer.Enable();
    
    HandleLeftTurnOff();
}
// !SECTION

void setup() 
{
    // SECTION - Setting the operating modes on the board pins
    pinMode(SPEEDOMETER_PIN, INPUT_PULLUP);
    pinMode(RIGHT_TURN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LEFT_TURN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LEFT_TURN_LAMP_PIN, OUTPUT);
    pinMode(RIGHT_TURN_LAMP_PIN, OUTPUT);
    pinMode(ERROR_SIGNAL_PIN, OUTPUT);
    pinMode(ROM_SAVE_SIGNAL_PIN, OUTPUT);
    // !SECTION

    // SECTION - Interruption handlers binding
    attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), HandleLeftTurnChangeState, CHANGE);
    attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), HandleRightTurnChangeState, CHANGE);
    attachInterrupt(SPEEDOMETER_PIN - 2, HandleSpeedometer, FALLING);
    // !SECTION

    Serial.begin(BAUD);
    SD.begin(ROM_PIN);

    while (!Serial) 
    {
      ; // NOTE - Wait for serial port to connect. Needed for native USB port only
    }

    if (!Memory::InitROM(card, volume, root))
        ErrorOccuring.BlinkForever();

    else if (Memory::GetFreeROM(volume) < Memory::minimal_free_rom_size)
        ErrorOccuring.BlinkForever(); 
}

void loop() 
{
    // SECTION - Saving data and then writing them to a file
    if (SaveReadingsTimer.IsPassed() && SaveReadingsTimer.IsEnabled() && IsInitialized)
    {
        last_reading.impulse_cnt = SpeedSensor.GetImpulseCount();
        last_reading.analog_voltage = analogRead(VOLTMETER_PIN);
        last_reading.speed_kmh = SpeedSensor.CalculateSpeed(SaveReadingsTimer.GetRepeatTime(), FrontWheel);
        last_reading.voltage_v = VoltageSensor.CalculateVoltage(last_reading.analog_voltage);

        if (Node::node_cnt == 0)
            head = Node::CreateHead(last_reading.impulse_cnt, last_reading.analog_voltage, millis_passed());
        
        else
            Node::Insert(head, last_reading.impulse_cnt, last_reading.analog_voltage, millis_passed());

        if (Node::node_cnt >= Node::max_node_cnt || Memory::GetFreeRAM() < Memory::minimal_free_ram_size)
        {
            Logger.WriteNodes(head);
            Node::DeleteAll(head);
        }

        SpeedSensor.ResetCounter(); 
        SaveReadingsTimer.ResetTime();
    }
    // !SECTION

    // SECTION - Output the last calculated value to the user
    if (SendReadingsTimer.IsPassed() && SendReadingsTimer.IsEnabled() && IsInitialized) 
    { 
        msg_temp = Message(last_reading.speed_kmh, last_reading.voltage_v);

        BluetoothAdapter::TransferMessage(msg_temp);
         
        SendReadingsTimer.ResetTime();
    }
    // !SECTION

    // SECTION - Processing of signals from the control device
    if (Serial.available() || IsAlert)
    {
        if (Serial.available())
        {
            msg_temp = BluetoothAdapter::RecieveMessage();
            
            if (!Message::IsValid(msg_temp))
                return;
        }

        else
            msg_temp = Message();

        if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::InitializationDone))
            IsInitialized = true;

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::StartSensorReadings))
            SendReadingsTimer.Enable();
        
        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::StopSensorReadings))
            SendReadingsTimer.Disable();

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::EnableLeftTurn))
            HandleLeftTurnOn();

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::EnableRightTurn))
            HandleRightTurnOn();

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::DisableTurns))
        {
            HandleLeftTurnOff();
            HandleRightTurnOff();
        }

        else if ((MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::Alert)) || IsAlert)
        {
            if (!IsAlert)
            {
                RightTurning.ChangeStateTimer.Disable();
                LeftTurning.ChangeStateTimer.Disable();
                IsAlert = true;
            }

            else
            {
                LeftTurning.ChangeStateTimer.Enable();
                RightTurning.ChangeStateTimer.Enable();
                IsAlert = false;
            }  
        }

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::GetNowDate))
        {   
            if (Logger.TrySetDateTime(msg_temp.data))
            {
                Logger.WriteHeader(VoltageSensor, FrontWheel, SaveReadingsTimer);
                millis_passed.initialization_time_ms = millis();
                if (!IsInitialized)
                {                    
                    msg_temp = Message(MessageAnalyzer::MessagePrefixes::Response, MessageAnalyzer::MessageCodes::SendModuleVersion, __MODULE_VERSION__);
                    BluetoothAdapter::TransferMessage(msg_temp);
                }
                else if (Node::node_cnt != 0)
                {
                    Node::DeleteAll(head);
                }
            }
        }
    }
    // !SECTION

    // SECTION - Flashing LEDs and turn signals
    if (SdCardSaving.IsInReactionInterval(Logger.GetLastWriteTime()) && !SdCardSaving.ChangeStateTimer.IsEnabled())
        SdCardSaving.ChangeStateTimer.Enable();
    
    else if (!SdCardSaving.IsInReactionInterval(Logger.GetLastWriteTime()) && SdCardSaving.ChangeStateTimer.IsEnabled())
        SdCardSaving.ChangeStateTimer.Disable();

    LeftTurning.TryBlink();
    RightTurning.TryBlink();
    SdCardSaving.TryBlink();
    // !SECTION
}
