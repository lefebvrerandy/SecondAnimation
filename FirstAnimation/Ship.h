#pragma once
#include "SpriteSheet.h"
#include "GameLevel.h"
#include <vector>

class Ship
{
private:
	float _shipSpeed;
	pair<float,float> _location;
	SpriteSheet* _spriteSheet;

public:
	Ship();
	virtual ~Ship();
	float GetShipSpeed();
	bool SetShipSpeed(float newSpeed);
	pair<float, float> GetLocation();
	bool SetLocation(pair<float, float> newLocation);
	wchar_t* ConvertToWCHAR_T(char* charToConvert);
	SpriteSheet* GetShipSprite();
	bool SetShipSprite(SpriteSheet* newSprite);

//char* imageFilePath, Graphics* gfx
};