#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include <string>


void Level1::Load() 
{
	mciSendString("open \"resources\\ds9_infirmary.mp3\" type mpegvideo alias background", NULL, 0, NULL);
	mciSendString("play background repeat", NULL, 0, NULL);
	// Load the spritesheets
	_background = new GameBoard(gfx);
	_pShip = new PlayerShip(gfx);
	_eShip = new EnemyShip(gfx);
	_planetSpawn = new Planet(gfx);

	// Create the ship effect
	_pShip->SetEffect(_pShip->GetSpriteObject()->ChromaAndScaleEffect(0, 1, 0));
	_pShip->SetDecalEffect(_pShip->GetDecal()->ChromaAndScaleEffect(0, 1, 0));

	// Create the enemy ship effect
	_eShip->SetEffect(_eShip->GetSpriteObject()->ChromaAndScaleEffect(0, 0, 1));

	// Create the planet effect
	_planetSpawn->SetEffect(_planetSpawn->GetSpriteObject()->ChromaAndScaleEffect(0, 1, 0));

	// Generate the grid so that we have a vector with all the spawning locations
	gfx->DrawGrid(GRID_STARTING_X, GRID_STARTING_Y, 1.0f, 1.0f, 1.0f, 1.0f, centerCoords);

	// Generate random planet locations
	ChangeUpPlanets();
}


void Level1::Unload()
{
	delete _background;
	delete _pShip;
	delete _eShip;
	delete _planetSpawn;
}


void Level1::Update()
{
	GameController::UpdatePlayerPosition2(_pShip);
	GameController::UpdateEnemyPosition(_eShip, _pShip);
	GameController::CollisionDetection(_pShip, _eShip);
	for (int i = 0; i < planetCoords.size(); i++)
	{
		if (GameController::CollisionDetection(_pShip, get<0>(planetCoords[i])))
		{
			planetCoords.erase(planetCoords.begin() + i);
			break;
		}
	}

}

void Level1::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.5f);

	// Render background
	_background->GetSpriteObject()->Draw();

	gfx->DrawPlayerStats(_pShip->GetEnergy(),_pShip->GetScience());

	// Render grid
	//gfx->DrawGrid(GRID_STARTING_X, GRID_STARTING_Y, 1.0f, 1.0f, 1.0f, 1.0f, centerCoords);

	// Render enemy ship
	for (int i = 0; i < planetCoords.size(); i++)
	{
		_planetSpawn = get<0>(planetCoords[i]);
		_planetSpawn->GetSpriteObject()->DrawEffect(_planetSpawn->GetEffect(), get<1>(planetCoords[i]), get<2>(planetCoords[i]));
	}

	// Render player ship
	_pShip->GetSpriteObject()->DrawEffect(_pShip->GetEffect(), _pShip->GetLocation().first, _pShip->GetLocation().second);
	_pShip->GetDecal()->DrawEffect(_pShip->GetDecalEffect(), _pShip->GetLocation().first, _pShip->GetLocation().second);

	// Render enemy ship
	_eShip->GetSpriteObject()->DrawEffect(_eShip->GetEffect(), _eShip->GetLocation().first, _eShip->GetLocation().second);
}

void Level1::ChangeUpPlanets()
{
	_planetSpawn->GenerateRandomPlanetLocation(centerCoords, planetCoords, gfx);
}
