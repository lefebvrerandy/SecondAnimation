#pragma once
#include "SpriteObject.h"

class Ship : public SpriteObject
{
private:
	float _shipSpeed;
	//pair<float,float> _location;
	//SpriteSheet* _spriteSheet;
	//ID2D1Effect* _effect;
	//ID2D1Bitmap* _bmp;

public:
	Ship();
	~Ship();

	float GetShipSpeed();
	bool SetShipSpeed(float newSpeed);

	virtual pair<float, float> StartingLocation() = 0;
};