#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include <string>


void Level1::Load() 
{
	//ShipBaseX = 0.0f;
	//ShipBaseX = ShipBaseYSpeed = 0.0f;

	startTimer = true;
	startedTime = 0;
	endingTime = 0;
	WaitTime = SHIP_DELAY;

	// convert all images to wchar_t*
	char* backgroundChar = (char*)"resources//SectorBackground.bmp";
	char* planet1Char = (char*)"resources//Planet1.bmp";
	char* planet2Char = (char*)"resources//Planet2.bmp";
	char* planet3Char = (char*)"resources//Planet3.bmp";
	//char* shipBaseChar = (char*)"resources//ShipBase.bmp";
	char* shipDetailsChar = (char*)"resources//ShipDetail.bmp";

	wchar_t* wbackground = ConvertToWCHAR_T(backgroundChar);
	wchar_t* wplanet1 = ConvertToWCHAR_T(planet1Char);
	wchar_t* wplanet2 = ConvertToWCHAR_T(planet2Char);
	wchar_t* wplanet3 = ConvertToWCHAR_T(planet3Char);
	//wchar_t* wshipbase = ConvertToWCHAR_T(shipBaseChar);
	wchar_t* wshipdetail = ConvertToWCHAR_T(shipDetailsChar);

	// Load the spritesheets
	background = new SpriteSheet(wbackground, gfx);
	planet1 = new SpriteSheet(wplanet1, gfx);
	planet2 = new SpriteSheet(wplanet2, gfx);
	planet3 = new SpriteSheet(wplanet3, gfx);
	/*PlayerShip */_pShip = PlayerShip(gfx);
	///shipBase = new SpriteSheet(wshipbase, gfx);
	//shipDetail = new SpriteSheet(wshipdetail, gfx);

	// Generate random planet locations
	GenerateRandomPlanetLocation(centerCoords, spawnCoords);

	// Find the ships starting and ending location
	pair<float, float> centerCoords = GridCenter();
	///shipStartingX = centerCoords.first - GRID_SIZE / 2;
	//shipStartingY = centerCoords.second;
	//ShipBaseX = shipStartingX;
}

wchar_t* Level1::ConvertToWCHAR_T(char* charToConvert)
{
	char* c = (char*)charToConvert;
	const size_t cSize = strlen(c) + 1;
	wchar_t* convertedImage = new wchar_t[cSize];
	mbstowcs(convertedImage, c, cSize);

	return convertedImage;
}


void Level1::Unload()
{
	delete background;
	//delete shipBase;
	//delete shipDetail;
	delete planet1;
	delete planet2;
	delete planet3;
}


void Level1::Update()
{

	/// Move the ship by one cube at a time at the rate of 0.5 seconds
	//ShipBaseYSpeed = GRID_SIZE/10;
	if (startTimer)
	{
		startedTime = clock();
		startTimer = false;
	}
	else
	{
		endingTime = clock();
		float elapsedTime = (endingTime - startedTime);
		if (elapsedTime >= WaitTime)
		{
			//if (ShipBaseX >= GRID_SIZE)
			//{
			//	ShipBaseX = shipStartingX;
			//	GenerateRandomPlanetLocation(centerCoords, spawnCoords);
			//}
			//else
			//{
			//	ShipBaseX += ShipBaseYSpeed;
			//}
			startTimer = true;
		}
	}


	/// Gradually move the y location by 2
	//ShipBaseYSpeed = 2.0f;
	//ShipBaseX += ShipBaseYSpeed;
	//if (ShipBaseX > (GRID_SIZE + GRID_STARTING_X))
	//{
	//	if (startTimer)
	//	{
	//		startedTime = clock();
	//		startTimer = false;
	//	}
	//	else
	//	{
	//		endingTime = clock();
	//		float elapsedTime = (endingTime - startedTime) / CLOCKS_PER_SEC;
	//		if (WaitTime <= elapsedTime)
	//		{
	//			ShipBaseX = shipStartingX; //keep the ball from dropping below the screen
	//			GenerateRandomCoordinates();
	//			startTimer = true;
	//		}
	//	}
	//}
}

void Level1::Render()
{
	PlayerShip _pShip();
	_pShip->DrawChroma;
	gfx->ClearScreen(0.0f, 0.0f, 0.5f);
	gfx->DrawGrid(GRID_STARTING_X, GRID_STARTING_Y, 1.0f, 1.0f, 1.0f, 1.0f, centerCoords);
	background->Draw();
	for (int i = 0; i < spawnCoords.size(); i++)
	{
		int planet = get<0>(spawnCoords[i]);
		switch (planet)
		{
			case 0:
				planet1->DrawChroma(get<1>(spawnCoords[i]), get<2>(spawnCoords[i]));
				break;
			case 1:
				planet2->DrawChroma(get<1>(spawnCoords[i]), get<2>(spawnCoords[i]));
				break;
			case 2:
				planet3->DrawChroma(get<1>(spawnCoords[i]), get<2>(spawnCoords[i]));
				break;
		}
	}

	pair<float, float> gridCenter = GridCenter();
	//shipBase->DrawChroma(ShipBaseX, shipStartingY);
	//shipDetail->DrawChroma(ShipBaseX, shipStartingY);
}


pair<float,float> Level1::GridCenter(void)
{
	float gridSize = GRID_SIZE;
	float gridStartingX = GRID_STARTING_X;
	float gridStartingY = GRID_STARTING_Y;

	pair<float, float> gridCenter;
	gridCenter.first = (GRID_SIZE / 2) + GRID_STARTING_X;
	gridCenter.second = (GRID_SIZE / 2) + GRID_STARTING_Y;

	return gridCenter;
}

void Level1::GenerateRandomPlanetLocation(vector<pair<float, float>>& centerCoords, vector<tuple<int, float, float>>& spawnLocations)
{
	float cellSize = GRID_SIZE / 10;
	spawnLocations.clear();
	for (int i = 0; i < centerCoords.size(); i++)
	{
		float xCenter = centerCoords[i].first;
		float yCenter = centerCoords[i].second;
		float xSpawnLocation = xCenter - cellSize/2 + GRID_STARTING_X;
		float ySpawnLocation = yCenter - cellSize/2 + GRID_STARTING_Y;


		// There is a 1 in 20 chance that each square will have a planet in it 
		int d = rand() % 20;
		if (d == 1)
		{
			int planet = rand() % 3;
			switch (planet)
			{
			case 0:
				spawnLocations.push_back(make_tuple(0, xSpawnLocation, ySpawnLocation));
				planet1->DrawChroma(xSpawnLocation, ySpawnLocation);
				break;
			case 1:
				spawnLocations.push_back(make_tuple(1, xSpawnLocation, ySpawnLocation));
				planet2->DrawChroma(xSpawnLocation, ySpawnLocation);
				break;
			case 2:
				spawnLocations.push_back(make_tuple(2, xSpawnLocation, ySpawnLocation));
				planet3->DrawChroma(xSpawnLocation, ySpawnLocation);
				break;
			}
		}
		else
		{
			spawnLocations.push_back(make_tuple(-1, xSpawnLocation, ySpawnLocation));
		}
	}
}