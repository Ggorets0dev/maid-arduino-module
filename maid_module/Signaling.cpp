#include "devices.h"

// * Creating Signaling with in initial modes of turns
Signaling::Signaling(Mode left_turn_mode, Mode right_turn_mode)
{
    this->IsLeftTurnEnabled = left_turn_mode;
    this->IsRightTurnEnabled = right_turn_mode;
	this->LeftTurnMillis = 0;
	this->RightTurnMillis = 0;
}

// * Check if the turn signal is on
bool Signaling::IsEnabled(Side side)
{
    if (side == Side::Left)
        return IsLeftTurnEnabled;
    
    else if (side == Side::Right)
        return IsRightTurnEnabled;
    
    else
        return false;
}

// * Enable left or right turn
void Signaling::EnableTurn(Side side)
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
void Signaling::DisableTurn(Side side)
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
