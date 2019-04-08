
#include "GameController.h"

GameLevel* GameController::currentLevel;
GameLevel* GameController::SaveState;
GameLevel* GameController::planetMenu;
bool GameController::Loading;
pair<float, float> GameController::_playerPositionDestination;
bool GameController::PlayerMoving;
PlayerShip* _player;
Planet* _planet;

void GameController::Init()
{
	Loading = true;
	currentLevel = 0;
}

void GameController::LoadInitialLevel(GameLevel* lev)
{
	Loading = true; //This can help us avoid loading activity while rendering
	currentLevel = lev;
	currentLevel->Load();
	mciSendString("open \"resources\\tos-computer-06.mp3\" type mpegvideo alias planetCollision", NULL, 0, NULL);
	mciSendString("open \"resources\\smallexplosion3.mp3\" type mpegvideo alias enemyCollision", NULL, 0, NULL);
	Loading = false;
}

void GameController::SwitchLevel(GameLevel* lev)
{
	Loading = true;
	currentLevel->Unload();
	lev->Load();
	delete currentLevel;
	currentLevel = lev;
	Loading = false;
}

GameLevel* GameController::GetCurrentLevel()
{
	return currentLevel;
}

void GameController::OpenPlanetMenu(GameLevel * lev)
{
	Loading = true;
	SaveState = currentLevel;
	lev->Load();
	//delete currentLevel;
	currentLevel = lev;
	Loading = false;
}

void GameController::ClosePlanetMenu(GameLevel * lev)
{
	Loading = true;
	currentLevel->Unload();
	//lev->Load();
	//delete currentLevel;
	currentLevel = lev;
	Loading = false;
}

void GameController::Render()
{
	if (Loading) return;//nice! Do not update or render if the scene is loading.
	currentLevel->Render();
}

void GameController::Update()
{
	if (Loading) return; //nice! Do not update or render if the scene is loading.
	currentLevel->Update();
}

void GameController::ProcessPlayerCoordinates(POINTS CursorPointer)
{
	_playerPositionDestination = make_pair(CursorPointer.x, CursorPointer.y);
}

void GameController::UpdatePlayerPosition2(PlayerShip* pShip)
{
	// We will first compare to see if the destination contains values.
	// If it does, this means the player wants to move. So lets move them
	// If it doesn't, the player doesnt want to move. Lets quit the method
	if ((_playerPositionDestination.first == NULL) && _playerPositionDestination.second == NULL)
	{
		PlayerMoving = false;
		return;
	}
	PlayerMoving = true;
	// Here will will store:
	// 1. Player current location
	// 2. Players speed
	// 3. Destination location
	// We store this information into variables so that we dont have to call the name each time throughout
	// the methods execution process
	pair<float, float> pos = pShip->GetLocation();
	float playerSpeed = pShip->GetShipSpeed();
	pair<float, float> des = make_pair(_playerPositionDestination.first, _playerPositionDestination.second);

	// Before any calculation, lets check that the player location isnt equal to the requested location. This
	// would make doing any calculations pointless
	if ((pos.first == des.first) && (pos.second == des.second))
	{
		PlayerMoving = false;
		return;
	}


	// find out the distance between the x and y coordinate. 
	vector<pair<bool,float>> DistanceDirection = CalculatateDistanceCoordinates(pos, des);
	// In DistanceDirection, we now have two items in the vector.
	// Two items that contain: bool - true means the first item is greater
	//						   float - distance between the two items
	// First item is the X Coordinates, Second item is the Y Coordinates


	//Now we can start moving the players coordinates

	// We'll use these two variables to determine which speed would be adjusted to move
	// the ship more fluidly
	float xIsGreater = 1;
	float yIsGreater = 1;
	if (DistanceDirection[0].first < DistanceDirection[1].second)
		xIsGreater = RatioBetweenPoints(DistanceDirection[0].second, DistanceDirection[1].second);
	else if (DistanceDirection[0].first > DistanceDirection[1].second)
		yIsGreater = RatioBetweenPoints(DistanceDirection[0].second, DistanceDirection[1].second);


	// Starting with the x coordinate. We check if we are moving left or right on the board.
	// DistanceDirection[0].first == true - means player position is positive compared to the cursor
	// This means we want to subtract the speed to move the player LEFT on the board
	if (_playerPositionDestination.first != NULL)
	{
			if (DistanceDirection[0].first == true)
			{
				if (pos.first - playerSpeed <= des.first)
				{
					pShip->SetLocation(make_pair(des.first, pos.second));
					_playerPositionDestination.first = NULL;
				}
				else
					pShip->SetLocation(make_pair(pos.first - playerSpeed, pos.second));
			}
			// This is the same as above, except to move the player RIGHT on the board
			else if (DistanceDirection[0].first == false)
			{
				if (pos.first + playerSpeed >= des.first)
				{
					pShip->SetLocation(make_pair(des.first, pos.second));
					_playerPositionDestination.first = NULL;
				}
				pShip->SetLocation(make_pair(pos.first + playerSpeed, pos.second));
			}
	}
	else
		_playerPositionDestination.first == NULL;



	pos = pShip->GetLocation();



	if (_playerPositionDestination.second != NULL)
	{
		if (DistanceDirection[1].first == true)
		{
			if (pos.second - playerSpeed <= des.second)
			{
				pShip->SetLocation(make_pair(pos.first, des.second));
				_playerPositionDestination.second = NULL;
			}
			else
				pShip->SetLocation(make_pair(pos.first, pos.second - playerSpeed));
		}
		else if (DistanceDirection[1].first == false)
		{
			if (pos.second + playerSpeed >= des.second)
			{
				pShip->SetLocation(make_pair(pos.first, des.second));
				_playerPositionDestination.second = NULL;
			}
			else
				pShip->SetLocation(make_pair(pos.first, pos.second + playerSpeed));
		}
	}
	else
		_playerPositionDestination.second == NULL;
}

void GameController::UpdatePlayerPosition(PlayerShip* pShip)
{
	// We will first compare to see if the destination contains values.
	// If it does, this means the player wants to move. So lets move them
	// If it doesn't, the player doesnt want to move. Lets quit the method
	if ((_playerPositionDestination.first == NULL) && (_playerPositionDestination.second == NULL))
		return;

	// Here will will store:
	// 1. Player current location
	// 2. Players speed
	// 3. Destination location
	// We store this information into variables so that we dont have to call the name each time throughout
	// the methods execution process
	pair<float, float> pos = pShip->GetLocation();
	float playerSpeed = pShip->GetShipSpeed();
	pair<float, float> des = make_pair(_playerPositionDestination.first, _playerPositionDestination.second);


	// This is where the logic of moving the player starts. The first if block is checking the X 
	// coordinate and the second if block is checking the Y coordinate.
	// We first start by making sure the destination coordinate is not equal to the player coordinate. 
	// These method if blocks will not run if the coordinates are the same. or the user doesnt need
	// to move any more of that specific X or Y value.
	// Both if blocks run the same but one is focused between X axis and the other is the Y axis.
	if ((des.first != pos.first) && (_playerPositionDestination.first != NULL))
	{
		// We first compare that adding or subtracting the speed to the destination does not equal
		// the players position. This will keep the gittering down
		if ((des.first + playerSpeed == pos.first) || (des.first - playerSpeed == pos.first))
			pShip->SetLocation(make_pair(des.first, pos.second));
		// If the destination is smaller than the players position
		else if (des.first < pos.first)
		{
			// Make sure that the destination doesnt go over than needed
			if (des.first + playerSpeed >= pos.first)
				pShip->SetLocation(make_pair(des.first, pos.second));
			else
				pShip->SetLocation(make_pair(pos.first - playerSpeed, pos.second));
		}
		else if (des.first > pos.first)
		{
			// Make sure that the destination doesnt go lower than needed
			if (des.first - playerSpeed <= pos.first)
				pShip->SetLocation(make_pair(des.first, pos.second));
			else
				pShip->SetLocation(make_pair(pos.first + playerSpeed, pos.second));
		}
		// A fail safe to keep the player position to NULL if an error happeneds
		else
			_playerPositionDestination.first = NULL;
	}
	// If we dont need to change the location we should NULL the destination to keep it from moving
	else
		_playerPositionDestination.first = NULL;

	// Lets update the pos before checking the Y axis
	pos = pShip->GetLocation();

	// This if block is identical to the above if block except it handles the Y access
	if ((des.second != pos.second) && (_playerPositionDestination.second != NULL))
	{
		if ((des.second + playerSpeed == pos.second) || (des.second - playerSpeed == pos.second))
			pShip->SetLocation(make_pair(pos.first, des.second));
		else if (des.second < pos.second)
		{
			if (des.second + playerSpeed >= pos.second)
				pShip->SetLocation(make_pair(pos.first, des.second));
			else
			pShip->SetLocation(make_pair(pos.first, pos.second - playerSpeed));
		}
		else if (des.second > pos.second)
		{
			if (des.second - playerSpeed <= pos.second)
				pShip->SetLocation(make_pair(pos.first, des.second));
			else
				pShip->SetLocation(make_pair(pos.first, pos.second + playerSpeed));
		}
		else
			_playerPositionDestination.second = NULL;
	}
	else
		_playerPositionDestination.second = NULL;


	pos = pShip->GetLocation();


	if ((des.first == pos.first) && (des.second == pos.second))
	{
		_playerPositionDestination.first = NULL;
		_playerPositionDestination.second = NULL;
	}



	//pShip->SetLocation(make_pair(des.first, des.second));
}

void GameController::UpdateEnemyPosition(EnemyShip * eShip, PlayerShip* pShip)
{
	// First we'll check if the player ship if not moving. 
	// If the player ship is not moving, we wont move the enemy
	if (!PlayerMoving)
		return;

	// If the player ship is moving, we must move the enemy ship
	// First start by finding where the player ship is compared to the enemy ship
	pair<float, float> pLocation = pShip->GetLocation();
	pair<float,float> eLocation = eShip->GetLocation();

	float distanceFromX = eLocation.first - pLocation.first;
	float distanceFromY = eLocation.second - pLocation.second;

	// If the players X location is greater than the enemy. Increase the enemy X by the ship speed
	if (pLocation.first > eLocation.first)
		eShip->SetLocation(make_pair(eLocation.first + eShip->GetShipSpeed(), eLocation.second));
	else if (pLocation.first < eLocation.first)
		eShip->SetLocation(make_pair(eLocation.first - eShip->GetShipSpeed(), eLocation.second));

	pLocation = pShip->GetLocation();
	eLocation = eShip->GetLocation();

	// If the players Y location is greater than the enemy. Increase the enemy Y by the ship speed
	if (pLocation.second > eLocation.second)
		eShip->SetLocation(make_pair(eLocation.first, eLocation.second + eShip->GetShipSpeed()));
	else if (pLocation.second < eLocation.second)
		eShip->SetLocation(make_pair(eLocation.first, eLocation.second - eShip->GetShipSpeed()));

	pLocation = pShip->GetLocation();
	eLocation = eShip->GetLocation();
}

bool GameController::CollisionDetection(SpriteObject * object1, SpriteObject * object2)
{
	// Check if the two objects are even close to eachother. If not, dont run the method
	float distanceFromX;
	float distanceFromY;

	if (Loading)
		return false;

	/// Every now and then, it thinks triggers planet menu after enemy collision.. Double check why

	if (object1->GetLocation().first > object2->GetLocation().first)
		distanceFromX = object1->GetLocation().first - object2->GetLocation().first;
	else
		distanceFromX = object2->GetLocation().first - object1->GetLocation().first;

	if (object1->GetLocation().second > object2->GetLocation().second)
		distanceFromY = object1->GetLocation().second - object2->GetLocation().second;
	else
		distanceFromY = object2->GetLocation().second - object1->GetLocation().second;

	if ((distanceFromX > 80) || (distanceFromY > 80))
		return false;

	// Check if object1 and object2 have collided
	pair<float, float> location1 = object1->GetLocation();
	pair<float, float> location2 = object2->GetLocation();


	// Stop the players movement
	_playerPositionDestination.first = NULL;
	_playerPositionDestination.second = NULL;

	// Check if the object is a planet. If so, we open the planet option
	// If not, we reset the player, enemy location and reduce players
	// energy by 300
	PlayerShip* x = (PlayerShip*)object1;
		if (dynamic_cast<Planet*>(object2))
		{
			if (dynamic_cast<Level1*>(currentLevel))
			{
				// Play sound when colliding with planet
				mciSendString("play planetCollision from 0", NULL, 0, NULL);
				PlanetMenu* menu = new PlanetMenu((Planet*)object2, (PlayerShip*)object1);
				_player = (PlayerShip*)object1;
				_planet = (Planet*)object2;
				OpenPlanetMenu(menu);	
			}
		}
		else
		{
			// Play sound when colliding with enemy
			mciSendString("play enemyCollision from 0", NULL, 0, NULL);
			// Set the location of both player and enemy to the starting positions
			x->SetLocation(x->StartingLocation());
			EnemyShip* y = (EnemyShip*)object2;
			y->SetLocation(y->StartingLocation());
			// Make sure the player takes damage
			x->TakeDamage(y->Attack());
			// Change up the planets
			Level1* z = (Level1*)currentLevel;
			z->ChangeUpPlanets();
		}
		return true;
}

vector<pair<bool, float>> GameController::CalculatateDistanceCoordinates(pair<float, float> item1, pair<float, float> item2)
{
	//Find the distance between the two as well as if
	// the players coordinats are greater or less than
	// the enemy.. This is used to find the direction to move
	float distanceX = 0;
	float distanceY = 0;
	bool greaterThanX = false;
	bool greaterThanY = false;
	if (item1.first > item2.first)
	{
		distanceX = item1.first - item2.first;
		greaterThanX = true;
	}
	else
	{
		distanceX = item2.first - item1.first;
		greaterThanX = false;
	}


	if (item1.second > item2.second)
	{
		distanceY = item1.second - item2.second;
		greaterThanY = true;
	}
	else
	{
		distanceY = item2.second - item1.second;
		greaterThanY = false;
	}

	// now that we know the distance between player and enemy both
	// X and Y, as well as where they are between each other, we can
	// return a tuple<greater,distanceX,distanceY>
	vector <pair<bool, float>> v;
	v.push_back(make_pair(greaterThanX, distanceX));
	v.push_back(make_pair(greaterThanY, distanceY));

	return v;
}

float GameController::RatioBetweenPoints(float item1, float item2)
{
	if ((item1 <= 0) || (item2 <= 0))
		return -1;

	float ratio = -1;
	if (item1 < item2)
		ratio = item2 / item1;
	else
		ratio = item1 / item2;

	return ratio;
}

void GameController::ProcessSelection(char selection)
{
	// If the selection is not between 1 - 3, we dont should get out of this method
	if ((selection != '1') && (selection != '2') && (selection != '3'))
		return;

		auto x = 0;
	if (selection == '1')
	{
		// We need to take the energy from the planet, and put it into the player	
		_player->SetEnergy(_player->GetEnergy() + _planet->GetEnergy());
	}
	else if (selection == '2')
	{
		// We need to take the science from the planet, and put it into the player
		_player->SetScience(_player->GetScience() + _planet->GetScience());
	}
	else if (selection == '3')
	{
		// We need to just exit the planet menu
		x = 3;
	}
	ClosePlanetMenu(SaveState);
}
