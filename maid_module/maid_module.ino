/*
  Project Name: Maid
  Package Name: maid-arduino-module
  Developer: Ggorets0dev
  Version: 0.0
  GitHub page: https://github.com/Ggorets0dev/maid-arduino-module
*/

#include "models.h"

// * ANALOG PINS USED:
#define VOLTMETER_PIN 0 
// * DIGITAL PINS USED:   
#define SPEEDOMETER_PIN 2      
#define RIGHT_TURN_BUTTON_PIN 4 
#define RIGHT_TURN_LAMP_PIN 5  
#define LEFT_TURN_BUTTON_PIN 6   
#define LEFT_TURN_LAMP_PIN 7    


// * Default settings and variables needed
unsigned long time_spent_from_start = 0;
const byte seconds_btw_transfer = 2; 
Models::Wheel FrontWheel(6, 2070);
Models::Instruments Multitool;


void setup() 
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() 
{
  if ((millis() - time_spent_from_start) / 1000 == seconds_btw_transfer) 
  {
    Serial.println(Multitool.CreateTransferMessage(60, 50));
    time_spent_from_start = millis(); 
  }
}
