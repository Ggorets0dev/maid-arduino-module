#include "devices.h"

// * Transfer message using bluetooth connection
void BluetoothAdapter::TransferMessage(const Message &msg, const AltSoftSerial &BtSerial)
{
    BtSerial.println(msg.ToString());
}

// * Recieve message using bluetooth connection
Message BluetoothAdapter::RecieveMessage(const AltSoftSerial &BtSerial)
{
    byte buffer[Message::maximal_message_length];
    BtSerial.readBytes(buffer, sizeof(buffer));
    const Message msg_temp = Message(String((char*)buffer));
    
	  return msg_temp;
}
