/*
  Project: MaidModule
  Developer: Ggorets0dev
  Version: 0.17.1
  GitHub: https://github.com/Ggorets0dev/maid-arduino-module
*/

#define __MODULE_VERSION__ "0.17.1"


#include <Arduino.h>
#include <AltSoftSerial.h>
#include "PinChangeInterrupt.h"

#include "config.h"
#include "pinout.h"
#include "typedefs.h"

#include "models.h" 
#include "devices.h"
#include "tools.h"


// NOTE - Variables-buffers for temporary storage
Reading last_reading;
Message msg_temp;

// NOTE - Setting up transfer ports
HardwareSerial &UsbSerial = Serial; 
AltSoftSerial BtSerial;

// NOTE - Variables for LED indicator feeds
Signal LeftTurning(LEFT_TURN_LAMP_PIN, static_cast<float>(TURN_SIGNAL_FLASH_DELAY_SEC), false, 1.0f);
Signal RightTurning(RIGHT_TURN_LAMP_PIN, static_cast<float>(TURN_SIGNAL_FLASH_DELAY_SEC), false, 1.0f);
Signal ErrorOccuring(ERROR_SIGNAL_PIN, 0.5f, false, 1.0f);
Signal SdCardSaving(ROM_SIGNAL_PIN, 0.20f, false, 2.0f);

// NOTE - Setting up to transmit and save readings
bool is_initialized = false;
Timer SendReadingsTimer(static_cast<float>(SEND_DELAY_SEC));
Timer SaveReadingsTimer(static_cast<float>(SAVE_DELAY_SEC));
DataSaver SdCard("DATA.TXT");
Logger EventLogger(UsbSerial);
MillisTracker millis_passed;
Sd2Card card;
SdVolume volume;
SdFile root;

// NOTE - Setting up a single-link list for storing information
uint Node::node_cnt = 0;
Node* head = nullptr;

// NOTE - Setting up devices to receive and calculate data
Wheel FrontWheel(WHEEL_SPOKES_CNT, WHEEL_CIRCUMFERENCE_MM);
Voltmeter VoltageSensor(MAX_VOLTAGE_V);
Speedometer SpeedSensor;

// SECTION - Interruption handlers
void ChangeStateLeftTurn()
{             
    if (LeftTurning.ChangeStateTimer.IsEnabled())
        LeftTurning.ChangeStateTimer.Disable(); 
    else
        LeftTurning.ChangeStateTimer.Enable();

    EventLogger.LogTurnStateChanged("LEFT");
}
void ChangeStateRightTurn()
{             
    if (RightTurning.ChangeStateTimer.IsEnabled())
        RightTurning.ChangeStateTimer.Disable(); 
    else
        RightTurning.ChangeStateTimer.Enable();

    EventLogger.LogTurnStateChanged("RIGHT");
}
void CountImpulse() 
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
    if (!static_cast<bool>(digitalRead(LEFT_TURN_BUTTON_PIN)))
        LeftTurning.ChangeStateTimer.Enable();
    if (!static_cast<bool>(digitalRead(RIGHT_TURN_BUTTON_PIN)))
        RightTurning.ChangeStateTimer.Enable();
    // !SECTION

    UsbSerial.begin(BAUD); BtSerial.begin(BAUD);
    SD.begin(ROM_DATA_PIN);

    if (!Memory::InitializeRom(card, volume, root) || Memory::GetFreeRom(volume) < Memory::minimal_free_rom_size)
        ErrorOccuring.BlinkForever(1.0f);
}

void loop() 
{
    // SECTION - Saving data and then writing them to a file
    if (SaveReadingsTimer.IsPassed() && SaveReadingsTimer.IsEnabled() && is_initialized)
    {
        last_reading.impulse_cnt = SpeedSensor.GetImpulseCount();
        last_reading.analog_voltage = analogRead(VOLTMETER_PIN);
        last_reading.speed_kmh = SpeedSensor.CalculateSpeed(SaveReadingsTimer.GetRepeatTime(), FrontWheel);
        last_reading.voltage_v = VoltageSensor.CalculateVoltage(last_reading.analog_voltage);

        if (Node::node_cnt == 0)
            head = Node::CreateHead(last_reading.impulse_cnt, last_reading.analog_voltage, millis_passed());
        
        else
            Node::Insert(head, last_reading.impulse_cnt, last_reading.analog_voltage, millis_passed());

        if (Node::node_cnt >= Node::max_node_cnt || Memory::GetFreeRam() < Memory::minimal_free_ram_size)
        {
            SdCard.WriteNodes(head);
            Node::DeleteAll(head);

            EventLogger.LogReadingsSaved();
        }

        SpeedSensor.ResetCounter(); 
        SaveReadingsTimer.ResetTime();
    }
    // !SECTION

    // SECTION - Output the last calculated value to the user
    if (SendReadingsTimer.IsPassed() && SendReadingsTimer.IsEnabled() && is_initialized) 
    { 
        msg_temp = Message(last_reading.speed_kmh, last_reading.voltage_v);
        BluetoothAdapter::TransferMessage(msg_temp, BtSerial);
         
        SendReadingsTimer.ResetTime();
        
        EventLogger.LogMsgSent();
    }
    // !SECTION

    // SECTION - Processing of signals from the control device
    if (BtSerial.available())
    {
        msg_temp = BluetoothAdapter::RecieveMessage(BtSerial);
        
        if (!Message::IsValid(msg_temp))
            msg_temp = Message();


        if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::LAUNCH_MODULE_CMD))
            is_initialized = true;

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::START_SENSOR_READINGS_CMD))
            SendReadingsTimer.Enable();
        
        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::STOP_SENSOR_READINGS_CMD))
            SendReadingsTimer.Disable();

        else if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::CURRENT_DATETIME_ENTRY))
        {   
            if (SdCard.TrySetDateTime(msg_temp.data))
            {
                SdCard.WriteHeader(VoltageSensor, FrontWheel, SaveReadingsTimer);
                millis_passed.SetMark(millis());
                
                if (!is_initialized)
                {                    
                    msg_temp = Message(MessageAnalyzer::MessagePrefixes::RESPONSE, MessageAnalyzer::MessageCodes::MODULE_VERSION_ENTRY, __MODULE_VERSION__);
                    BluetoothAdapter::TransferMessage(msg_temp, BtSerial);

                    EventLogger.LogMsgSent();
                }
                else if (Node::node_cnt != 0)
                {
                    Node::DeleteAll(head);
                }

                EventLogger.LogHeaderSaved();
            }
        }
        
        EventLogger.LogMsgRecived();
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
