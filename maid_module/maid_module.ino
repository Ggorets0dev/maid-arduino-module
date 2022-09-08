/*
  Project Name: MaidModule
  Repository Name: maid-arduino-module
  Developer: Ggorets0dev
  Version: 0.5
  GitHub page: https://github.com/Ggorets0dev/maid-arduino-module
*/

#include <Arduino.h>
#include "PinChangeInterrupt.h"
#include "pinout.h"
#include "typedefs.h"
#include "models.h" 
#include "devices.h"
#include "tools.h"


ulong time_since_launch_ms = 0;
byte msg_buffer[20];

const String VERSION = "0.5";
const byte SENSORS_READINGS_TRANSFER_DELAY_SEC = 2;
const uint BAUD = 9600; 

Wheel FrontWheel(6, 2070);
Voltmeter VoltageSensor(10, 100);
Speedometer SpeedSensor;
Message msg_recived;


// * Creating interrupt handlers for all available interrupts
void HandleSpeedometer(void) { SpeedSensor.CountImpulse(); }
void HandleLeftTurnSignMakerOn(void) { Signaler::ManipulateSingleSignal(Signaler::Side::Left, Signaler::Interaction::On); }
void HandleLeftTurnSignMakerOff(void) { Signaler::ManipulateSingleSignal(Signaler::Side::Left, Signaler::Interaction::Off); }
void HandleRightTurnSignMakerOn(void) { Signaler::ManipulateSingleSignal(Signaler::Side::Right, Signaler::Interaction::On); }
void HandleRightTurnSignMakerOff(void) { Signaler::ManipulateSingleSignal(Signaler::Side::Right, Signaler::Interaction::Off); }


void setup() 
{
    Serial.begin(BAUD);

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
    if ((millis() - time_since_launch_ms) / 1000 == SENSORS_READINGS_TRANSFER_DELAY_SEC) 
    {
        BluetoothAdapter::TransferMessage(Message(SpeedSensor.CalculateSpeed(SENSORS_READINGS_TRANSFER_DELAY_SEC, FrontWheel), VoltageSensor.CalculateVoltage(analogRead(VOLTMETER_PIN))));
        SpeedSensor.ResetCounter(); 

        time_since_launch_ms = millis();
    }

    if (Serial.available())
    {
        Serial.readBytes(msg_buffer, sizeof(msg_buffer));
        msg_recived = Message(String((char*)msg_buffer));
        memset(msg_buffer, 0, sizeof(msg_buffer));

        Serial.println(msg_recived.ToString());

        if (MessageAnalyzer::IsRequest(msg_recived) && MessageAnalyzer::IsCodeMatch(msg_recived, MessageAnalyzer::MessageCodes::GetModuleVersion))
            Serial.println(Message(MessageAnalyzer::MessagePrefixes::Response, MessageAnalyzer::MessageCodes::SendModuleVersion, VERSION).ToString());
    }
}
