#pragma once
#include "Ship.h"

class PlayerShip : Ship
{
private:
	SpriteSheet* _decal;

public:
	PlayerShip(Graphics* gfx);
	~PlayerShip();
	SpriteSheet* GetDecal();
	bool SetDecals(SpriteSheet* newDecal);

};