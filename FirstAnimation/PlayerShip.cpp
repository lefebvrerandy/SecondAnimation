#include "PlayerShip.h"

PlayerShip::PlayerShip(Graphics* gfx)
{
	SetShipSpeed(0);

	int halfOfGrid = GRID_SIZE / 2;
	SetLocation(make_pair(halfOfGrid, halfOfGrid));

	char* shipBaseChar = (char*)"resources//ShipBase.bmp";
	wchar_t* wshipbase = ConvertToWCHAR_T(shipBaseChar);
	SpriteSheet* playerSprite = new SpriteSheet(wshipbase, gfx);
	SetShipSprite(playerSprite);
}

PlayerShip::~PlayerShip()
{
	delete GetShipSprite();
}

SpriteSheet * PlayerShip::GetDecal()
{
	return nullptr;
}

bool PlayerShip::SetDecals(SpriteSheet * newDecal)
{
	return false;
}
