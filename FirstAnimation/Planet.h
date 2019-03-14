#pragma once
#include "SpriteObject.h"
#include <vector>
#include <tuple>

class Planet : public SpriteObject
{
private:
	int _energy;
	int _science;

public:
	Planet(Graphics* gfx);
	~Planet();

	float GetEnergy();
	bool SetEnergy(float newEnergy);
	float GetScience();
	bool SetScience(float newScience);
	char* GenerateRandomPlanet();
	void GenerateRandomPlanetLocation(vector<pair<float, float>>& centerCoords, vector<tuple<Planet*, float, float>>& planetCoords, Graphics* gfx);
	void GenerateRandomStats();

};