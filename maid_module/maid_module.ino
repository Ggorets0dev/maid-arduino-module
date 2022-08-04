/*
  Project Name: Maid
  Package Name: maid-arduino-module
  Developer: Ggorets0dev
  Version: 0.1
  GitHub page: https://github.com/Ggorets0dev/maid-arduino-module
*/

#include "PinChangeInterrupt.h" 
#include "models.h"
#include "pinout.h"


// * Default settings and variables needed
unsigned long time_spent_from_start = 0;
const byte seconds_btwn_transfer = 2; 

Models::Wheel FrontWheel(6, 2070);
Models::BluetoothAdapter HC_06;
Models::Speedometer Speedometer;
Models::Voltmeter Voltmeter(10, 100);
Models::Signaler Signaler;


// * Create interrupt handlers for all available interrupts
void HandleSpeedometerInterrupt(void) { Speedometer.CountImpulse(); }
void HandleLeftTurnSignalOn(void) { Signaler.TurnOnLeftSignal();  }
void HandleLeftTurnSignalOff(void) { Signaler.TurnOffLeftSignal(); }
void HandleRightTurnSignalOn(void) { Signaler.TurnOnRightSignal(); }
void HandleRightTurnSignalOff(void) { Signaler.TurnOffRightSignal(); }


void setup() 
{
  Serial.begin(9600);
  
  attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), HandleLeftTurnSignalOn, RISING);
  attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), HandleLeftTurnSignalOff, FALLING);

  attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), HandleRightTurnSignalOn, RISING);
  attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), HandleRightTurnSignalOff, FALLING);

  attachPCINT(digitalPinToPCINT(SPEEDOMETER_PIN), HandleSpeedometerInterrupt, FALLING);
}

void loop() 
{
  if ((millis() - time_spent_from_start) / 1000 == seconds_btwn_transfer) 
  {
    HC_06.TransferRecords(Speedometer.CalculateSpeed(seconds_btwn_transfer, FrontWheel), Voltmeter.CalculateVoltage(analogRead(VOLTMETER_PIN)));
    Speedometer.ResetCounter();
    time_spent_from_start = millis(); 
  }
}
