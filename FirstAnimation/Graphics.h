#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")

using namespace std;

#include<Windows.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <d2d1.h>
#include <d3d11_1.h>
#include <d2d1effects.h>
#include <d2d1effects_1.h>
#include <d2d1effects_2.h>
#include <d2d1effecthelpers.h>
#include "GameDimensions.h"
#include <vector>


class Graphics
{
	//Below, these are all COM interfaces we're using to create D2D resources.
	//We release them as part of the ~Graphics deconstructor... or bad things can happen
	ID2D1Factory* factory; //The factory allows us to create many other types of D2D resources
	ID2D1HwndRenderTarget* rendertarget; //this is typically an area in our GPU memory.. like a back buffer 
	ID2D1SolidColorBrush* brush; //Note this COM interface! Remember to release it!

	
public:
	Graphics();

	~Graphics();

	bool Init(HWND windowHandle);

	ID2D1RenderTarget* GetRenderTarget()
	{
		return rendertarget;
	}

	ID2D1DeviceContext* GetDeviceContext()
	{
		ID2D1DeviceContext *rtDC;
		rendertarget->QueryInterface(&rtDC);
		return rtDC;
	}

	void BeginDraw() 
	{
		rendertarget->BeginDraw(); 
	}

	void EndDraw() 
	{ 
		rendertarget->EndDraw(); 
	}

	void ClearScreen(float r, float g, float b);
	void DrawGrid(float x, float y, float r, float g, float b, float a, vector<pair<float, float>>& centerCoords);
};
