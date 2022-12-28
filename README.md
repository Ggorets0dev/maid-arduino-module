# MaidModule (Arduino)
Module based on the Arduino, which collects motorcycle data and partially controls peripherals. It is controlled by a special [MaidApp](https://github.com/Ggorets0dev/maid-android-application) *(Motorcycle Auxiliary Information Device)* written for Android.


## Installation
---
### **Libraries**

> **Warning:** All of the specified libraries must be loaded manually and be visible in the program for downloading sketches. They must also be available at the time of writing the sketch on board.

List of third-party libraries used in the sketch:
* [PinChangeInterrupt](https://www.arduino.cc/reference/en/libraries/pinchangeinterrupt/)
  

### **Manual**

Download the source code and, without changing the project structure, load the **maid_module.ino** sketch onto the board. All header files will be loaded automatically.

## Connection (Bluetooth)
---
### **Transfer messages (Sent/Received)**

You can find a table with all the possible messages on the [MaidApp](https://github.com/Ggorets0dev/maid-android-application/blob/master/README.md) page in the section **Transfer messages (Sent/Received)**.

## Coursework
---
This software (only this repository) was exhibited as a coursework in the 2nd year of the 1st semester of 09.03.03 "Applied Computer Science" in the discipline "Computer Science" at the Moscow Institute of Electronic Technology. All files related to the coursework can be found in a separate folder with the same name.