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

pair<float, float> Ship::GetLocation()
{
	return _location;
}

bool Ship::SetLocation(pair<float, float> newLocation)
{
	_location = newLocation;
	return true;
}

wchar_t * Ship::ConvertToWCHAR_T(char * charToConvert)
{
	char* c = (char*)charToConvert;
	const size_t cSize = strlen(c) + 1;
	wchar_t* convertedImage = new wchar_t[cSize];
	mbstowcs(convertedImage, c, cSize);

	return convertedImage;
}

SpriteSheet* Ship::GetShipSprite()
{
	return _spriteSheet;
}

bool Ship::SetShipSprite(SpriteSheet* newSpriteSheet)
{
	_spriteSheet = newSpriteSheet;
	return true;
}
