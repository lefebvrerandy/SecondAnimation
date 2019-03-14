#include "SpriteObject.h"

pair<float, float> SpriteObject::GetLocation()
{
	return _location;
}

bool SpriteObject::SetLocation(pair<float, float> newLocation)
{
	_location = newLocation;
	return true;
}

wchar_t * SpriteObject::ConvertToWCHAR_T(char * charToConvert)
{
	char* c = (char*)charToConvert;
	const size_t cSize = strlen(c) + 1;
	wchar_t* convertedImage = new wchar_t[cSize];
	mbstowcs(convertedImage, c, cSize);

	return convertedImage;
}

SpriteSheet* SpriteObject::GetSpriteObject()
{
	return _spriteSheet;
}

bool SpriteObject::SetSpriteObject(SpriteSheet * newSprite)
{
	_spriteSheet = newSprite;
	return false;
}

SpriteSheet * SpriteObject::ImageToSprite(char * imagePath, Graphics* gfx)
{
	char* charImage = imagePath;
	wchar_t* wSpriteObjectbase = ConvertToWCHAR_T(charImage);
	SpriteSheet* playerSprite = new SpriteSheet(wSpriteObjectbase, gfx);
	return playerSprite;
}

ID2D1Effect * SpriteObject::GetEffect()
{
	return _effect;
}

bool SpriteObject::SetEffect(ID2D1Effect * newEffect)
{
	_effect = newEffect;
	return true;
}

ID2D1Bitmap * SpriteObject::GetBmp()
{
	return _bmp;
}

bool SpriteObject::SetBmp(ID2D1Bitmap * newBmp)
{
	_bmp = newBmp;
	return true;
}
