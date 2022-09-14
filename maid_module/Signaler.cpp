#include "devices.h"

Signaler::Signaler(bool left_turn_mode, bool right_turn_mode)
{
    this->IsLeftTurnEnabled = left_turn_mode;
    this->IsRightTurnEnabled = right_turn_mode;
}

bool Signaler::IsEnabled(Side side)
{
    if (side == Side::Left)
        return IsLeftTurnEnabled;
    
    else if (side == Side::Right)
        return IsRightTurnEnabled;
    
    else
        return false;
}

void Signaler::EnableTurn(Side side)
{
    digitalWrite(side, HIGH);

    if (side == Side::Left)
        IsLeftTurnEnabled = true;

    else if (side == Side::Right)
        IsRightTurnEnabled = true;
}

void Signaler::DisableTurn(Side side)
{
    digitalWrite(side, LOW);

    if (side == Side::Left)
        IsLeftTurnEnabled = false;

    else if (side == Side::Right)
        IsRightTurnEnabled = false;
}