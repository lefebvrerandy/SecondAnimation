#include "PlayerShip.h"

PlayerShip::PlayerShip(Graphics* gfx)
{
	SetShipSpeed(10);
	_energy = 800;
	_science = 0;
	
	SetLocation(StartingLocation());

	// Get the sprite sheet for the player ship
	SpriteSheet* playerSprite = ImageToSprite((char*)"resources//ShipBase.bmp", gfx);
	SetSpriteObject(playerSprite);

	// Get the sprite sheet for the player ship decal
	SpriteSheet* playerSpriteDecal = ImageToSprite((char*)"resources//ShipDetail.bmp", gfx);
	SetDecals(playerSpriteDecal);
}

PlayerShip::~PlayerShip()
{
	delete _decal;
}

SpriteSheet * PlayerShip::GetDecal()
{
	return _decal;
}

bool PlayerShip::SetDecals(SpriteSheet * newDecal)
{
	_decal = newDecal;
	return true;
}

pair<float,float> PlayerShip::StartingLocation()
{
	int XGrid = GRID_STARTING_X + 100;
	int YGrid = GRID_SIZE / 2 + GRID_STARTING_Y;
	return make_pair(XGrid, YGrid);
}

ID2D1Effect * PlayerShip::GetDecalEffect()
{
	return _decalEffect;
}

bool PlayerShip::SetDecalEffect(ID2D1Effect * newEffect)
{
	_decalEffect = newEffect;
	return true;
}

ID2D1Bitmap * PlayerShip::GetDecalBmp()
{
	return _decalBmp;
}

bool PlayerShip::SetDecalBmp(ID2D1Bitmap * newBmp)
{
	_decalBmp = newBmp;
	return true;
}

float PlayerShip::GetEnergy()
{
	return _energy;
}

bool PlayerShip::SetEnergy(float newEnergy)
{
	_energy = newEnergy;
	if (_energy <= 0)
		_energy = 0;
	else if (_energy >= 800)
		_energy = 800;
	return true;
}

float PlayerShip::GetScience()
{
	return _science;
}

bool PlayerShip::SetScience(float newScience)
{
	_science = newScience;
	return true;
}

void PlayerShip::TakeDamage(float damage)
{
	SetEnergy(_energy - damage);
}
