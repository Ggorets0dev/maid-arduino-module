/*
  Project Name: Maid
  Package Name: maid-arduino-module
  Developer: Ggorets0dev
  Version: 0.2
  GitHub page: https://github.com/Ggorets0dev/maid-arduino-module
*/

#include "PinChangeInterrupt.h" 
#include "models.h"
#include "pinout.h"


// * Default settings and variables needed
ulong time_spent_from_start = 0;
const byte seconds_btwn_transfer = 2; 

const Models::Wheel FrontWheel(6, 2070);
Models::BluetoothAdapter HC_06;
Models::Speedometer Speedometer;
Models::Voltmeter Voltmeter(10, 100);
Models::Signaler Signaler;


// * Create interrupt handlers for all available interrupts
void HandleSpeedometerInterrupt(void) { Speedometer.CountImpulse(); }
void HandleLeftTurnSignalOn(void) { Signaler.ManipulateSingleSignal(Signaler.SignalSide::Left, Signaler.SignalInteraction::On); }
void HandleLeftTurnSignalOff(void) { Signaler.ManipulateSingleSignal(Signaler.SignalSide::Left, Signaler.SignalInteraction::Off); }
void HandleRightTurnSignalOn(void) { Signaler.ManipulateSingleSignal(Signaler.SignalSide::Right, Signaler.SignalInteraction::On); }
void HandleRightTurnSignalOff(void) { Signaler.ManipulateSingleSignal(Signaler.SignalSide::Right, Signaler.SignalInteraction::Off); }


void setup() 
{
  Serial.begin(9600);
  
  attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), HandleLeftTurnSignalOn, RISING);
  attachPCINT(digitalPinToPCINT(LEFT_TURN_BUTTON_PIN), HandleLeftTurnSignalOff, FALLING);

  attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), HandleRightTurnSignalOn, RISING);
  attachPCINT(digitalPinToPCINT(RIGHT_TURN_BUTTON_PIN), HandleRightTurnSignalOff, FALLING);

  attachInterrupt(SPEEDOMETER_PIN, HandleSpeedometerInterrupt, FALLING);
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
