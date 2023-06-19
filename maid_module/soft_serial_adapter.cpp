#include "devices.h"

// * Transfer message using bluetooth connection
void SoftSerialAdapter::TransferMessage(const Message &msg)
{
    this->SoftSerial.println(msg.ToString());
}

// * Recieve message using bluetooth connection
Message SoftSerialAdapter::RecieveMessage()
{
    byte buffer[Message::maximal_message_length];
    this->SoftSerial.readBytes(buffer, sizeof(buffer));
    const Message msg_temp = Message(String((char*)buffer));
    
	  return msg_temp;
}
