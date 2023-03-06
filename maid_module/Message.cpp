#include "models.h"

Message::Message(String msg)
{
    this->prefix = msg[0];
    this->code = msg.substring(msg.indexOf("{") + 1, msg.indexOf("}")).toInt();
    this->data = msg.substring(msg.indexOf("}") + 1, msg.indexOf(";"));
}

Message::Message(char prefix, int code, String data)
{
    this->prefix = prefix;
    this->code = code;
    this->data = data;
}

Message::Message(float speed, float voltage)
{
    this->prefix = MessageAnalyzer::MessagePrefixes::Response;
    this->code = MessageAnalyzer::MessageCodes::SensorReadingEntry;
    this->data = String(speed, 2) + "_" + String(voltage, 2);
}

Message::Message() 
{
    this->prefix = MessageAnalyzer::MessagePrefixes::EmptyPrefix;
    this->code = MessageAnalyzer::MessageCodes::EmptyCode;
    this->data = EMPTY_STRING;
}

String Message::ToString() const
{
    return String(prefix) + "{" + String(code) + "}" + data + ";";
}

// * Check if the message matches the template 
bool Message::IsValid(const Message &msg)
{
    const bool prefix_check = msg.prefix == MessageAnalyzer::MessagePrefixes::EmptyPrefix ||
                            msg.prefix == MessageAnalyzer::MessagePrefixes::Request ||
                            msg.prefix == MessageAnalyzer::MessagePrefixes::Response;

    const bool code_check = msg.code >= MessageAnalyzer::MessageCodes::EmptyCode && 
                            msg.code <= MessageAnalyzer::MessageCodes::ModuleLaunchCmd;

    const bool data_check = msg.data.length() >= 0 && 
                            msg.data.length() <= Message::maximal_message_length;

    return prefix_check && code_check && data_check;
}
