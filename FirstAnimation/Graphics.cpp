#include "Graphics.h"

/***********************************************************************************
The intent of the Graphics class is to handle our DirectX calls, and to be largely responsible
for managing the rendertarget.
******************************************************************************************/

//Constructor for Graphics class
Graphics::Graphics()
{
	factory = NULL;
	rendertarget = NULL;
	brush = NULL;
}

//Destructor for Graphics class
//Note that all COM objects we instantiate should be 'released' here 
//Look for comments on COM usage in the corresponding header file.

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (rendertarget) rendertarget->Release();
	if (brush) brush->Release();
}

//Provide some comments for each of the methods below.
//Be sure you get a sense of what is happening, and resolve any issues you have understanding these
// methods, their parameters, returns and so on.
bool Graphics::Init(HWND windowHandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect); //set the rect's right and bottom properties = the client window's size

	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&rendertarget);
	if (res != S_OK) return false;

	res = rendertarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
	if (res != S_OK) return false;
	return true;
}

void Graphics::ClearScreen(float r, float g, float b)
{
	rendertarget->Clear(D2D1::ColorF(r, g, b));
}

//void Graphics::DrawGrid(float x, float y, float r, float g, float b, float a, int centerPoint[][10])
void Graphics::DrawGrid(float x, float y, float r, float g, float b, float a, vector<pair<float, float>>& centerCoords)
{
	float BoxSize = (GRID_SIZE / 10);
	centerCoords.clear();
	brush->SetColor(D2D1::ColorF(r, g, b, a));

	int j = 0;
	int i = 0;
	int k = 0;
	for (int j = 0; j < 10; j++)
	{
		int arrayY = (j * BoxSize);
		for (int i = 0; i < 10; i++)
		{
			int arrayX = (i * BoxSize);
			rendertarget->DrawLine(D2D1::Point2F(x +		   arrayX, y +		     arrayY), D2D1::Point2F(x +		      arrayX, y + BoxSize + arrayY), brush, 3.0f);
			rendertarget->DrawLine(D2D1::Point2F(x +		   arrayX, y + BoxSize + arrayY), D2D1::Point2F(x + BoxSize + arrayX, y + BoxSize + arrayY), brush, 3.0f);
			rendertarget->DrawLine(D2D1::Point2F(x + BoxSize + arrayX, y + BoxSize + arrayY), D2D1::Point2F(x + BoxSize + arrayX, y +			arrayY), brush, 3.0f);
			rendertarget->DrawLine(D2D1::Point2F(x + BoxSize + arrayX, y +		     arrayY), D2D1::Point2F(x +			  arrayX, y +			arrayY), brush, 3.0f);

			// Store the center of the square into the array for later use
			//centerCoords.push_back(make_pair((float)(x + arrayX/2), (float)(y + arrayY/2)));
			centerCoords.push_back(make_pair((float)(BoxSize/2 + arrayY), (float)BoxSize/2 + arrayX));
			
			k++;
		}

	}
	int o = 0;
}