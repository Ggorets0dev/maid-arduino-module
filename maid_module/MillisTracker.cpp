#include "tools.h"

ulong MillisTracker::operator()()
{
    return millis() - initialization_time_ms;
}