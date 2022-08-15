#include "models.h"
#include <Arduino.h>

namespace Models
{
    Wheel::Wheel(byte spokes, ushort size_mm)
    { 
        count_of_spokes = spokes; 
        wheel_circumference_mm = size_mm; 
    }
    
    byte Wheel::GetSpokesCount() 
    { 
        return count_of_spokes; 
    }
    
    ushort Wheel::GetWheelCircumference() 
    { 
        return wheel_circumference_mm; 
    }   
}