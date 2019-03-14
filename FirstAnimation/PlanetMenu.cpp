#include "PlanetMenu.h"

PlanetMenu::PlanetMenu(Planet * planet, PlayerShip * player)
{
	if (player != NULL)
		_player = player;
	if (planet != NULL)
		_planet = planet;
}

void PlanetMenu::Load()
{
	_background = new GameBoard(gfx);
}

void PlanetMenu::Unload()
{
}

void PlanetMenu::Update()
{
}

void PlanetMenu::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.5f);

	// Render background
	_background->GetSpriteObject()->Draw();
	gfx->DrawPlayerStats(_player->GetEnergy(), _player->GetScience());

	float halfX = RESOLUTION_X / 2;
	float halfY = RESOLUTION_Y / 2;

	gfx->DrawMenu(halfX, halfY, 1.0f, 1.0f, 1.0f, 1.0f);
	gfx->DrawMenuText(_planet->GetEnergy(),_planet->GetScience());
}
