#pragma once
#pragma warning(disable : 4996)

#include "GameLevel.h"
#include "GameDimensions.h"
#include "SpriteObject.h"
#include "SpriteSheet.h"
#include <vector>


class SpriteObject
{
private:
	pair<float, float> _location;
	SpriteSheet* _spriteSheet;
	ID2D1Effect* _effect;
	ID2D1Bitmap* _bmp;

public:
	virtual ~SpriteObject() = default;
	pair<float, float> GetLocation();
	bool SetLocation(pair<float, float> newLocation);

	wchar_t* ConvertToWCHAR_T(char* charToConvert);
	SpriteSheet* GetSpriteObject();
	bool SetSpriteObject(SpriteSheet* newSprite);
	SpriteSheet* ImageToSprite(char * imagePath, Graphics* gfx);

	ID2D1Effect* GetEffect();
	bool SetEffect(ID2D1Effect* newEffect);

	ID2D1Bitmap* GetBmp();
	bool SetBmp(ID2D1Bitmap* newBmp);
};