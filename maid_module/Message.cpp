#include "models.h"

Message::Message(String msg)
{
    this->prefix = msg[0];
    this->code = msg.substring(msg.indexOf('{') + 1, msg.indexOf('}')).toInt();
    this->data = msg.substring(msg.indexOf('}') + 1, msg.indexOf(';'));
}

Message::Message(float speed, float voltage)
{
    this->prefix = MessageAnalyzer::MessagePrefixes::RESPONSE;
    this->code = MessageAnalyzer::MessageCodes::SENSOR_READING_ENTRY;
    
    this->data = String(speed, 2) + 
    this->data += F("_");
    this->data += String(voltage, 2);
}

Message::Message() 
{
    this->prefix = MessageAnalyzer::MessagePrefixes::EMPTY_PREFIX;
    this->code = MessageAnalyzer::MessageCodes::EMPTY_CODE;
    this->data = EMPTY_STRING;
}

String Message::ToString() const
{
    String message = String(this->prefix);
    message += F("{");
    message += this->code;
    message += F("}");
    message += this->data;
    message += F(";");

    return message;
}

// * Check if the message matches the template 
bool Message::IsValid(const Message &msg)
{
    const bool prefix_check = msg.prefix == MessageAnalyzer::MessagePrefixes::EMPTY_PREFIX ||
                            msg.prefix == MessageAnalyzer::MessagePrefixes::REQUEST ||
                            msg.prefix == MessageAnalyzer::MessagePrefixes::RESPONSE;

    const bool code_check = msg.code >= MessageAnalyzer::MessageCodes::EMPTY_CODE && 
                            msg.code <= MessageAnalyzer::MessageCodes::LAUNCH_MODULE_CMD;

    const bool data_check = msg.data.length() >= 0 && 
                            msg.data.length() <= Message::maximal_message_length;

    return prefix_check && code_check && data_check;
}
