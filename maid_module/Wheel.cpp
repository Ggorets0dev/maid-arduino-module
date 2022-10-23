#include "models.h"

Wheel::Wheel(byte spokes_cnt, uint wheel_circ_mm)
{ 
    this->count_of_spokes = spokes_cnt; 
    this->wheel_circumference_mm = wheel_circ_mm; 
}