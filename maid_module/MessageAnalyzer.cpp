#include "tools.h"

// * Checking if message is request
bool MessageAnalyzer::IsRequest(Message &msg)
{
    return msg.prefix == MessagePrefixes::Request;
}

// * Checking if message is response
bool MessageAnalyzer::IsResponse(Message &msg)
{
    return msg.prefix == MessagePrefixes::Response;
}

// * Checking if code in message matches code sent as argument
bool MessageAnalyzer::IsCodeMatch(Message &msg, MessageCodes code)
{
    return msg.code == code;
}

