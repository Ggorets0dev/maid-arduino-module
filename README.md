# MaidModule (Arduino)
Module based on the Arduino, which collects motorcycle data and partially controls peripherals. It is controlled by a special [MaidApp](https://github.com/Ggorets0dev/maid-android-application) *(Motorcycle Auxiliary Information Device)* written for Android.

<p align='center'>
       <img height=275 src="materials/Maid_Logo.png"/>
</p>


## Operating principle
---
### **Connection**
MaidModule communicates with the MaidApp via Bluetooth. **HC-06** bluetooth module is responsible for communication.


### **Transfer messages (Sent/Received)**

You can find a table with all the possible messages on the [MaidApp](https://github.com/Ggorets0dev/maid-android-application/blob/master/README.md) page in the section **Transfer messages (Sent/Received)**.