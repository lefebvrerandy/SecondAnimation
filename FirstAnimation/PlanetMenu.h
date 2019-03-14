#pragma once
#include "GameLevel.h"
#include "GameBoard.h"
#include "Planet.h"
#include "PlayerShip.h"

class PlanetMenu : public GameLevel
{
private:
	GameBoard* _background;
	Planet* _planet;
	PlayerShip* _player;

public:
	PlanetMenu(Planet* planet, PlayerShip* player);
	void Load() override;
	void Unload() override;
	void Update() override;
	void Render() override;
};