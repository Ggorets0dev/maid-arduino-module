#include "tools.h"

bool MessageAnalyzer::IsRequest(Message msg)
{
    return msg.GetPrefix() == MessagePrefixes::Request;
}

bool MessageAnalyzer::IsResponse(Message msg)
{
    return msg.GetPrefix() == MessagePrefixes::Response;
}

bool MessageAnalyzer::IsCodeMatch(Message msg, MessageCodes code)
{
    return msg.GetCode() == code;
}

