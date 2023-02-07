#include "devices.h"

// * Transfer message using bluetooth connection
void BluetoothAdapter::TransferMessage(Message &msg)
{
    Serial.println(msg.ToString());
}

// * Recieve message using bluetooth connection
Message BluetoothAdapter::RecieveMessage()
{
    byte buffer[Message::maximal_message_length];

    Serial.readBytes(buffer, sizeof(buffer));

    Serial.println(String((char*)buffer));

    Message msg_temp = Message(String((char*)buffer));
    
    return msg_temp;
}
