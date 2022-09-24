#include "devices.h"

// * Creating signaler with in itial values
Signaler::Signaler(bool left_turn_mode, bool right_turn_mode)
{
    this->IsLeftTurnEnabled = left_turn_mode;
    this->IsRightTurnEnabled = right_turn_mode;
	this->LeftTurnMillis = 0;
	this->RightTurnMillis = 0;
}

// * Check if the turn signal is on
bool Signaler::IsEnabled(Side side)
{
    if (side == Side::Left)
        return IsLeftTurnEnabled;
    
    else if (side == Side::Right)
        return IsRightTurnEnabled;
    
    else
        return false;
}

// * Enable left or right turn
void Signaler::EnableTurn(Side side)
{
    digitalWrite(side, HIGH);

    if (side == Side::Left)
	{
		IsLeftTurnEnabled = true;
		LeftTurnMillis = millis();
	}

    else if (side == Side::Right)
	{
		IsRightTurnEnabled = true;
		RightTurnMillis = millis();
	}
}

// * Disable left or right turn
void Signaler::DisableTurn(Side side)
{
    digitalWrite(side, LOW);

    if (side == Side::Left)
    {
		IsLeftTurnEnabled = false;
		LeftTurnMillis = millis();
	}


    else if (side == Side::Right)
	{
		IsRightTurnEnabled = false;
		RightTurnMillis = millis();
	}
}
