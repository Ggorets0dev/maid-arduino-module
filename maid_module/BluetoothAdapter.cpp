#include "devices.h"

// * Transfer message using bluetooth connection
void BluetoothAdapter::TransferMessage(Message &msg)
{
    Serial.println(msg.ToString());
}
