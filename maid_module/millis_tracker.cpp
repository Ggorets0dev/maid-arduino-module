#include "tools.h"

void MillisTracker::SetMark(ulong time_ms)
{
    this->mark_time_ms = time_ms;
}

ulong MillisTracker::operator()()
{
    return millis() - this->mark_time_ms;
}
