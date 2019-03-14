#include "Ship.h"

Ship::Ship()
{
}

Ship::~Ship()
{
}

float Ship::GetShipSpeed()
{
	return _shipSpeed;
}

bool Ship::SetShipSpeed(float newSpeed)
{
	_shipSpeed = newSpeed;
	return true;
}
