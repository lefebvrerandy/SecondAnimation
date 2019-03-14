#pragma once

#include <wincodec.h> //This is the WIC codec header - we need this to decode image files
#include "Graphics.h" //This includes both Windows and D2D libraries
//Remember to add "windowscodecs.lib" to your Linker/Input/AdditionalDependencies
#include <utility>

class SpriteSheet
{
	Graphics* gfx; //Reference to the Graphics class
	ID2D1Bitmap* bmp; //This will hold our loaded and converted Bitmap file


public:
	//Constructor
	SpriteSheet(wchar_t* filename, Graphics* gfx);

	//Destructor
	~SpriteSheet();

	//Draw bitmap to the render target
	void DrawEffect(ID2D1Effect* effect, float x = 0, float y = 0);
	void Draw();
	ID2D1Effect* ChromaEffect(float r, float g, float b);
	ID2D1Effect* ChromaAndScaleEffect(/*Graphics* gfx,*/ float r, float g, float b);
	ID2D1Effect* MergeEffect(ID2D1Effect* effect1, ID2D1Effect* effect2);
	ID2D1Bitmap* GetBitmap();
	bool SetBitmap(ID2D1Bitmap* newBmp);
	void DrawBmp(ID2D1Bitmap* bmp);

};