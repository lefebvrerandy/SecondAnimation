#pragma once
#include "GameLevel.h"
#include "GameDimensions.h"
#include "GameBoard.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "Planet.h"
#include <ctime>
#include <vector>
#include <tuple>



class Level1 : public GameLevel
{
	//float _energy;
	//float _science;

	vector<pair<float, float>> centerCoords;
	vector<tuple<Planet*, float, float>> planetCoords;

	GameBoard* _background;
	PlayerShip* _pShip;
	EnemyShip* _eShip;
	Planet* _planetSpawn;


public:
	void Load() override;
	void Unload() override;
	void Update() override;
	void Render() override;
	void ChangeUpPlanets();
};
