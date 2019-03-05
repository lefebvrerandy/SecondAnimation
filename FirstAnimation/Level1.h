#pragma once
#include "GameLevel.h"
#include "GameDimensions.h"
#include "PlayerShip.h"
#include <ctime>
#include <vector>
#include <tuple>



class Level1 : public GameLevel
{
	bool startTimer;
	clock_t startedTime;
	clock_t endingTime;
	clock_t WaitTime;

	//float ShipBaseX;
	//float ShipBaseYSpeed;
	//float shipStartingX;
	//float shipStartingY;

	//float shipBaseX;
	//float shipBaseY;

	vector<pair<float, float>> centerCoords;
	vector<tuple<int, float, float>> spawnCoords;

	SpriteSheet* background;
	SpriteSheet* planet1;
	SpriteSheet* planet2;
	SpriteSheet* planet3;
	//SpriteSheet* shipBase;
	//SpriteSheet* shipDetail;


public:
	void Load() override;
	wchar_t* ConvertToWCHAR_T(char* charToConvert);
	void Unload() override;
	void Update() override;
	void Render() override;
	pair<float, float> GridCenter(void);
	void GenerateRandomPlanetLocation(vector<pair<float, float>>& centerCoords, vector<tuple<int, float, float>>& spawnLocations);
};
