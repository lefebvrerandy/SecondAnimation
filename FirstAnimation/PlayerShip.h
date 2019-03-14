#pragma once
#include "Ship.h"

class PlayerShip : public Ship
{
private:
	SpriteSheet* _decal;
	ID2D1Effect* _decalEffect;
	ID2D1Bitmap* _decalBmp;
	float _energy;
	float _science;

public:
	PlayerShip(Graphics* gfx);
	~PlayerShip();
	SpriteSheet* GetDecal();
	bool SetDecals(SpriteSheet* newDecal);
	pair<float, float> StartingLocation() override;
	ID2D1Effect* GetDecalEffect();
	bool SetDecalEffect(ID2D1Effect* newEffect);
	ID2D1Bitmap* GetDecalBmp();
	bool SetDecalBmp(ID2D1Bitmap* newBmp);
	float GetEnergy();
	bool SetEnergy(float newEnergy);
	float GetScience();
	bool SetScience(float newScience);
	void TakeDamage(float damage);
};