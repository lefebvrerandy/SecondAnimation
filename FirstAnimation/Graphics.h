#pragma once
#pragma warning(disable : 4996)

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
#include <dwrite.h>
#include "GameDimensions.h"
#include <vector>
#include <stdio.h>


class Graphics
{
	//Below, these are all COM interfaces we're using to create D2D resources.
	//We release them as part of the ~Graphics deconstructor... or bad things can happen
	ID2D1Factory* factory; //The factory allows us to create many other types of D2D resources

	// https://docs.microsoft.com/en-us/windows/desktop/directwrite/getting-started-with-directwrite
	ID2D1Factory* m_pD2DFactory;
	IDWriteFactory* DWriteFactory; 
	IDWriteTextFormat* pTextFormat;
	ID2D1SolidColorBrush* pBlackBrush;

	ID2D1HwndRenderTarget* rendertarget; //this is typically an area in our GPU memory.. like a back buffer 
	ID2D1SolidColorBrush* brush; //Note this COM interface! Remember to release it!
	ID2D1SolidColorBrush* SolidGrayBrush;

	
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
	//void DrawGrid(float x, float y, float r, float g, float b, float a, vector<pair<float, float>>& centerCoords);
	void DrawGrid(float x, float y, float r, float g, float b, float a, vector<pair<float, float>>& centerCoords);
	void DrawMenu(float x, float y, float r, float g, float b, float a);
	D2D1_ROUNDED_RECT CreateRect(D2D1_SIZE_F rtSize, float left, float top, float right, float bottom);
	bool DrawMenuText(float energy, float science);
	void DrawTextToMenu(D2D1_SIZE_F rtSize, const wchar_t* string, unsigned int size, float left, float top, float right, float bottom);
	void DrawPlayerStats(float energy, float science);
};
