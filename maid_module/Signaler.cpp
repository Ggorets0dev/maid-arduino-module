#include "devices.h"

void Signaler::ManipulateSingleSignal(Side side, Interaction mode) 
{ 
    digitalWrite(side, mode); 
}