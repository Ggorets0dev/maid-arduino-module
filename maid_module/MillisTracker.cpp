#include "tools.h"

ulong MillisTracker::operator()()
{
    return millis() - this->initialization_time_ms;
}