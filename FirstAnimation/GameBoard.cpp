#include "GameBoard.h"

GameBoard::GameBoard(Graphics* gfx)
{
	SpriteSheet* planetSprite = ImageToSprite((char*)"resources//SectorBackground.bmp", gfx);
	SetSpriteObject(planetSprite);
}

GameBoard::~GameBoard()
{
}
