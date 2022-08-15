#include "models.h"
#include "transfer_prefixes.h"
#include <Arduino.h>

namespace Models
{
    void BluetoothAdapter::TransferMessage(String msg, char prefix=READINGS_PREFIX)
    {
        // * "!" is temporary prefix
        String cmd = " !" + msg + ";"; cmd[1] = prefix;
        Serial.println(cmd);
    }
    
    void BluetoothAdapter::TransferRecords(float speed, float voltage, char prefix=READINGS_PREFIX)
    {
        // * "!" is temporary prefix
        String cmd = " !" + String(speed, 2) + "_" + String(voltage, 2) + ";"; cmd[1] = prefix;
        Serial.println(cmd);
    }
}