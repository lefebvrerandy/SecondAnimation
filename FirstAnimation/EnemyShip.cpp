#include "EnemyShip.h"

EnemyShip::EnemyShip(Graphics * gfx)
{
	SetShipSpeed(6);
	SetLocation(StartingLocation());
	SetDamage(300);

	SpriteSheet* enemySprite = ImageToSprite((char*)"resources//EnemyShip.bmp", gfx);
	SetSpriteObject(enemySprite);
}

EnemyShip::~EnemyShip()
{
}

pair<float,float> EnemyShip::StartingLocation()
{
	int XGrid = GRID_SIZE - 100;
	int YGrid = GRID_SIZE / 2 + GRID_STARTING_Y;
	return make_pair(XGrid, YGrid);
}

float EnemyShip::Attack()
{
	return _damage;
}

void EnemyShip::SetDamage(float newDamage)
{
	_damage = newDamage;
}
