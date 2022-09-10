#include "devices.h"

void BluetoothAdapter::TransferMessage(Message &msg)
{
    Serial.println(msg.ToString());
}
