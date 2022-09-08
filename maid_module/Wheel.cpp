#include "models.h"

Wheel::Wheel(byte spokes, uint size_mm)
{ 
    this->count_of_spokes = spokes; 
    this->wheel_circumference_mm = size_mm; 
}

byte Wheel::GetSpokesCount() 
{ 
    return count_of_spokes; 
}

uint Wheel::GetWheelCircumference() 
{ 
    return wheel_circumference_mm; 
}   