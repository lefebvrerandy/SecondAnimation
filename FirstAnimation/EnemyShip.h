#pragma once
#include "Ship.h"

class EnemyShip : public Ship
{
private:
	pair<float, float> distanceFromPlayer;
	float _damage;

public:
	EnemyShip(Graphics* gfx);
	~EnemyShip();
	pair<float, float> StartingLocation() override;
	float Attack();
	void SetDamage(float newDamage);
};