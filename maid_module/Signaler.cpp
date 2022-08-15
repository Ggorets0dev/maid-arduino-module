#include "models.h"

namespace Models
{
    void Signaler::ManipulateSingleSignal(Side side, Interaction mode) 
    { 
        digitalWrite(side, mode); 
    }
}