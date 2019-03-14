#pragma once

#include "GameLevel.h"
#include "Graphics.h"
#include "SpriteObject.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "PlanetMenu.h"
#include "Planet.h"
#include "Level1.h"
#include <vector>

//This will be a Singleton class (constructor is private)

class GameController
{
	GameController() {}
	static GameLevel* currentLevel;
	static GameLevel* SaveState;
	static GameLevel* planetMenu;

public:
	static pair<float,float> _playerPositionDestination;
	static bool PlayerMoving;
	static bool Loading;
	static void Init();
	static void LoadInitialLevel(GameLevel* lev);
	static void SwitchLevel(GameLevel* lev);
	static GameLevel* GetCurrentLevel();
	static void OpenPlanetMenu(GameLevel* lev);
	static void ClosePlanetMenu(GameLevel* lev);
	static void Render();
	static void Update();
	static void ProcessPlayerCoordinates(POINTS CursorPointer);
	static void UpdatePlayerPosition2(PlayerShip* pShip);
	static void UpdatePlayerPosition(PlayerShip* pShip);
	static void UpdateEnemyPosition(EnemyShip* eShip, PlayerShip* pShip);
	static bool CollisionDetection(SpriteObject* object1, SpriteObject* object2);
	static vector<pair<bool, float>> CalculatateDistanceCoordinates(pair<float, float> item1, pair<float, float> item2);
	static float RatioBetweenPoints(float item1, float item2);
	static void ProcessSelection(char selection);
};