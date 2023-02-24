#include "devices.h"

// * Transfer message using bluetooth connection
void BluetoothAdapter::TransferMessage(Message &msg, AltSoftSerial &BtSerial)
{
    BtSerial.println(msg.ToString());
}

// * Recieve message using bluetooth connection
Message BluetoothAdapter::RecieveMessage(AltSoftSerial &BtSerial)
{
    byte buffer[Message::maximal_message_length];
    BtSerial.readBytes(buffer, sizeof(buffer));
    Message msg_temp = Message(String((char*)buffer));
    
	  return msg_temp;
}
