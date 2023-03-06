/*
  Project: MaidModule
  Repository: maid-arduino-module
  Developer: Ggorets0dev
  Version: 0.16.0
  GitHub page: https://github.com/Ggorets0dev/maid-arduino-module
*/

#define __MODULE_VERSION__ "0.16.0"

#include <Arduino.h>
#include <AltSoftSerial.h>
#include "PinChangeInterrupt.h"
#include "pinout.h"
#include "typedefs.h"
#include "models.h" 
#include "devices.h"
#include "tools.h"

// NOTE - Variables-buffers for temporary storage
Reading last_reading;
Message msg_temp;

// NOTE - Setting up transfer ports
const uint BAUD = 9600;
const HardwareSerial &UsbSerial = Serial; 
const AltSoftSerial BtSerial;

// NOTE - Variables for LED indicator feeds
const Signal LeftTurning(LEFT_TURN_LAMP_PIN, 1.0f, false);
const Signal RightTurning(RIGHT_TURN_LAMP_PIN, 1.0f, false);
const Signal ErrorOccuring(ERROR_SIGNAL_PIN, 0.5f, false);
const Signal SdCardSaving(ROM_SIGNAL_PIN, 0.20f, false, 2.0f);

// NOTE - Setting up to transmit and save readings
bool IsInitialized = false;
const Timer SendReadingsTimer(2.0f);
const Timer SaveReadingsTimer(0.5f);
const DataSaver SdCard("data.txt");
MillisTracker millis_passed;
Sd2Card card;
SdVolume volume;
SdFile root;

// NOTE - Setting up a single-link list for storing information
uint Node::node_cnt = 0;
Node* head = NULL;

// NOTE - Setting up devices to receive and calculate data
const Wheel FrontWheel(8, 2070);
const Voltmeter VoltageSensor(60);
const Speedometer SpeedSensor(0);

// SECTION - Interruption handlers
void ChangeStateLeftTurn(void)
{             
    if (LeftTurning.ChangeStateTimer.IsEnabled())
        LeftTurning.ChangeStateTimer.Disable(); 
    else
        LeftTurning.ChangeStateTimer.Enable();

    Logger::LogTurnStateChanged("LEFT");
}
void ChangeStateRightTurn(void)
{             
    if (RightTurning.ChangeStateTimer.IsEnabled())
        RightTurning.ChangeStateTimer.Disable(); 
    else
        RightTurning.ChangeStateTimer.Enable();

    Logger::LogTurnStateChanged("RIGHT");
}
void CountImpulse(void) 
{ 
    SpeedSensor.CountImpulse(); 
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
    pinMode(ROM_SIGNAL_PIN, OUTPUT);
    // !SECTION

    // SECTION - Interruption handlers binding
    attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), ChangeStateLeftTurn, CHANGE);
    attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), ChangeStateRightTurn, CHANGE);
    attachInterrupt(SPEEDOMETER_PIN - 2, CountImpulse, FALLING);
    // !SECTION

    // SECTION - Installing the original state of the turn signals
    if (!(bool)digitalRead(LEFT_TURN_BUTTON_PIN))
        LeftTurning.ChangeStateTimer.Enable();
    if (!(bool)digitalRead(RIGHT_TURN_BUTTON_PIN))
        RightTurning.ChangeStateTimer.Enable();
    // !SECTION

    UsbSerial.begin(BAUD); BtSerial.begin(BAUD);
    SD.begin(ROM_DATA_PIN);

    if (!Memory::InitROM(card, volume, root) || Memory::GetFreeROM(volume) < Memory::minimal_free_rom_size)
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
            SdCard.WriteNodes(head);
            Node::DeleteAll(head);

            Logger::LogReadingsSaved();
        }

        SpeedSensor.ResetCounter(); 
        SaveReadingsTimer.ResetTime();
    }
    // !SECTION

    // SECTION - Output the last calculated value to the user
    if (SendReadingsTimer.IsPassed() && SendReadingsTimer.IsEnabled() && IsInitialized) 
    { 
        msg_temp = Message(last_reading.speed_kmh, last_reading.voltage_v);
        BluetoothAdapter::TransferMessage(msg_temp, BtSerial);
         
        SendReadingsTimer.ResetTime();
        
        Logger::LogMsgSent();
    }
    // !SECTION

    // SECTION - Processing of signals from the control device
    if (BtSerial.available())
    {
        msg_temp = BluetoothAdapter::RecieveMessage(BtSerial);
        
        if (!Message::IsValid(msg_temp))
            msg_temp = Message();


        if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::ModuleLaunchCmd))
            IsInitialized = true;

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::StartSensorReadingsCmd))
            SendReadingsTimer.Enable();
        
        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::StopSensorReadingsCmd))
            SendReadingsTimer.Disable();

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::CurrentDateTimeEntry))
        {   
            if (SdCard.TrySetDateTime(msg_temp.data))
            {
                SdCard.WriteHeader(VoltageSensor, FrontWheel, SaveReadingsTimer);
                millis_passed.initialization_time_ms = millis();
                
                if (!IsInitialized)
                {                    
                    msg_temp = Message(MessageAnalyzer::MessagePrefixes::Response, MessageAnalyzer::MessageCodes::ModuleVersionEntry, __MODULE_VERSION__);
                    BluetoothAdapter::TransferMessage(msg_temp, BtSerial);

                    Logger::LogMsgSent();
                }
                else if (Node::node_cnt != 0)
                {
                    Node::DeleteAll(head);
                }

                Logger::LogHeaderSaved();
            }
        }
        
        Logger::LogMsgRecived();
    }
    // !SECTION

    // SECTION - Flashing LEDs and turn signals
    if (SdCardSaving.IsInReactionInterval(SdCard.GetLastWriteTime()) && !SdCardSaving.ChangeStateTimer.IsEnabled())
        SdCardSaving.ChangeStateTimer.Enable();
    
    else if (!SdCardSaving.IsInReactionInterval(SdCard.GetLastWriteTime()) && SdCardSaving.ChangeStateTimer.IsEnabled())
        SdCardSaving.ChangeStateTimer.Disable();

    LeftTurning.TryBlink();
    RightTurning.TryBlink();
    SdCardSaving.TryBlink();
    // !SECTION
}
