#include "Planet.h"

//Constructor for planet
Planet::Planet(Graphics* gfx)
{

	SpriteSheet* planetSprite = ImageToSprite(GenerateRandomPlanet(), gfx);
	SetSpriteObject(planetSprite);
	GenerateRandomStats();

}

//Deconstructor for planet
Planet::~Planet()
{
}

float Planet::GetEnergy()
{
	return _energy;
}

bool Planet::SetEnergy(float newEnergy)
{
	_energy = newEnergy;
	return true;
}

float Planet::GetScience()
{
	return _science;
}

bool Planet::SetScience(float newScience)
{
	_science = newScience;
	return true;
}

char * Planet::GenerateRandomPlanet()
{
	int whichPlanet = 0;
	char* planetFileLocation = NULL;

	whichPlanet = rand() % 3;
	switch (whichPlanet)
	{
		case 0:
			planetFileLocation = (char*)"resources//Planet1.bmp";
			break;
		case 1:
			planetFileLocation = (char*)"resources//Planet2.bmp";
			break;
		case 2:
			planetFileLocation = (char*)"resources//Planet3.bmp";
			break;	
	}

	return planetFileLocation;
}

void Planet::GenerateRandomPlanetLocation(vector<pair<float, float>>& centerCoords, vector<tuple<Planet*, float, float>>& planetCoords, Graphics* gfx)
{
	float cellSize = GRID_SIZE / 10;
	planetCoords.clear();

	for (int i = 0; i < centerCoords.size(); i++)
	{
		int containsPlanet = rand() % 20;
		switch (containsPlanet)
		{
		case 1:
			// Spawn a new planet
			Planet* x = new Planet(gfx);
			// Set the planets effect (chroma key)
			x->SetEffect(x->GetSpriteObject()->ChromaAndScaleEffect(0, 1, 0));
			// Set the location for the planet
			x->SetLocation(make_pair(centerCoords[i].first, centerCoords[i].second));
			// Store the planet into the list <- this is used to render the planets on the board
			planetCoords.push_back(make_tuple(x, centerCoords[i].first, centerCoords[i].second));

			break;
		}
	}
}

void Planet::GenerateRandomStats()
{
	SetEnergy(rand() % 300);
	SetScience(rand() % 300);
}
