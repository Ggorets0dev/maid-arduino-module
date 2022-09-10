/*
  Project: MaidModule
  Repository: maid-arduino-module
  Developer: Ggorets0dev
  Version: 0.5.1
  GitHub page: https://github.com/Ggorets0dev/maid-arduino-module
*/

#define __MODULE_VERSION__ "0.5.1"


#include <Arduino.h>
#include "PinChangeInterrupt.h"
#include "pinout.h"
#include "typedefs.h"
#include "models.h" 
#include "devices.h"
#include "tools.h"


ulong time_since_launch_ms = 0;
byte msg_byte_buffer[20];

const byte SENSORS_READINGS_TRANSFER_DELAY_SEC = 2;
const uint BAUD = 9600; 

Wheel FrontWheel(6, 2070);
Voltmeter VoltageSensor(10, 100);
Speedometer SpeedSensor(0);
Message msg_temp;


// * Creating interrupt handlers for all available interrupts
void HandleSpeedometer(void) { SpeedSensor.CountImpulse(); }
void HandleLeftTurnSignMakerOn(void) { Signaler::ManipulateSingleSignal(Signaler::Side::Left, Signaler::Interaction::On); }
void HandleLeftTurnSignMakerOff(void) { Signaler::ManipulateSingleSignal(Signaler::Side::Left, Signaler::Interaction::Off); }
void HandleRightTurnSignMakerOn(void) { Signaler::ManipulateSingleSignal(Signaler::Side::Right, Signaler::Interaction::On); }
void HandleRightTurnSignMakerOff(void) { Signaler::ManipulateSingleSignal(Signaler::Side::Right, Signaler::Interaction::Off); }


void setup() 
{
    Serial.begin(BAUD);

    // randomSeed(analogRead(0)); For testing readings transfering

    pinMode(SPEEDOMETER_PIN, INPUT_PULLUP);
    pinMode(RIGHT_TURN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LEFT_TURN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LEFT_TURN_LAMP_PIN, OUTPUT);
    pinMode(RIGHT_TURN_LAMP_PIN, OUTPUT);
    
    attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), HandleLeftTurnSignMakerOn, RISING);
    attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), HandleRightTurnSignMakerOn, RISING);
    attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), HandleRightTurnSignMakerOff, FALLING);
    attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), HandleLeftTurnSignMakerOff, FALLING);
    attachInterrupt(0, HandleSpeedometer, FALLING);
}

void loop() 
{
    if ((millis() - time_since_launch_ms) / 1000 >= SENSORS_READINGS_TRANSFER_DELAY_SEC) 
    {
        //msg_temp = Message((float)random(99), (float)random(99)); For testing readings transfering

        msg_temp =  Message(SpeedSensor.CalculateSpeed(SENSORS_READINGS_TRANSFER_DELAY_SEC, FrontWheel), VoltageSensor.CalculateVoltage(analogRead(VOLTMETER_PIN)));
        
        BluetoothAdapter::TransferMessage(msg_temp);
        
        SpeedSensor.ResetCounter(); 

        time_since_launch_ms = millis();
    }

    if (Serial.available())
    {
        Serial.readBytes(msg_byte_buffer, sizeof(msg_byte_buffer));
        msg_temp = Message(String((char*)msg_byte_buffer));
        memset(msg_byte_buffer, 0, sizeof(msg_byte_buffer));

        Serial.println(msg_temp.ToString());

        if (MessageAnalyzer::IsRequest(msg_temp) && MessageAnalyzer::IsCodeMatch(msg_temp, MessageAnalyzer::MessageCodes::GetModuleVersion))
        {
            msg_temp = Message(MessageAnalyzer::MessagePrefixes::Response, MessageAnalyzer::MessageCodes::SendModuleVersion, __MODULE_VERSION__);
            BluetoothAdapter::TransferMessage(msg_temp);
        }
    }
}
