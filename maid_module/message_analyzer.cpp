#include "tools.h"

// * Checking if message is request
bool MessageAnalyzer::IsRequest(const Message &msg)
{
    return msg.prefix == MessagePrefixes::REQUEST;
}

// * Checking if message is response
bool MessageAnalyzer::IsResponse(const Message &msg)
{
    return msg.prefix == MessagePrefixes::RESPONSE;
}

// * Checking if code in message matches code sent as argument
bool MessageAnalyzer::IsCodeMatch(const Message &msg, MessageCodes code)
{
    return msg.code == code;
}

