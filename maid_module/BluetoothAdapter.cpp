#include "devices.h"

void BluetoothAdapter::TransferMessage(Message msg)
{
    String buffer = " " + msg.ToString();
    Serial.println(buffer);
}
