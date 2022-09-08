#include "models.h"

Wheel::Wheel(byte spokes, uint size_mm)
{ 
    count_of_spokes = spokes; 
    wheel_circumference_mm = size_mm; 
}

byte Wheel::GetSpokesCount() 
{ 
    return count_of_spokes; 
}

uint Wheel::GetWheelCircumference() 
{ 
    return wheel_circumference_mm; 
}   