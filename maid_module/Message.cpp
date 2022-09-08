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
    this->code = MessageAnalyzer::MessageCodes::GetSensorReadings;
    this->data = String(speed, 2) + "_" + String(voltage, 2);
}

Message::Message() 
{
    this->prefix = MessageAnalyzer::MessagePrefixes::EmptyPrefix;
    this->code = MessageAnalyzer::MessageCodes::EmptyCode;
    this->data = "";
}


int Message::GetCode()
{ 
    return code;
}

char Message::GetPrefix()
{
    return prefix;
}

String Message::GetData()
{
    return data;
}

String Message::ToString()
{
    return String(prefix) + "{" + String(code) + "}" + data + ";";
}
