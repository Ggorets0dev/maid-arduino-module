# MaidModule (Arduino)
The **Maid** *(Motorcycle Auxiliary Information Device)* module on the arduino, which collects motorcycle data and partially controls peripherals. It is controlled by a special [mobile application](https://github.com/Ggorets0dev/maid-android-application) written for Android.

<p align='center'>
       <img height=275 src="materials/Maid_Logo.png"/>
</p>


## Structure
---
### **Connecting Principle**

MaidModule communicates with the MaidApp via Bluetooth. **HC-06** bluetooth module is responsible for communication.


### **Types of messages (Sent/Received)**

You can find a table with all the possible messages on the MaidApp [markdown file](https://github.com/Ggorets0dev/maid-android-application/blob/master/README.md) in the section **Types of messages (Sent/Received)**.