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
	SolidGrayBrush = NULL;

	m_pD2DFactory = NULL;
	DWriteFactory = NULL;
	pTextFormat = NULL;
	pBlackBrush = NULL;
}

//Destructor for Graphics class
//Note that all COM objects we instantiate should be 'released' here 
//Look for comments on COM usage in the corresponding header file.

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (rendertarget) rendertarget->Release();
	if (brush) brush->Release();
	if (SolidGrayBrush) SolidGrayBrush->Release();
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
//void Graphics::DrawGrid(float x, float y, float r, float g, float b, float a, vector<pair<float, float>>& centerCoords)
//{
//	float BoxSize = (GRID_SIZE / 10);
//	centerCoords.clear();
//	brush->SetColor(D2D1::ColorF(r, g, b, a));
//
//	int j = 0;
//	int i = 0;
//	int k = 0;
//	for (int j = 0; j < 10; j++)
//	{
//		int arrayY = (j * BoxSize);
//		for (int i = 0; i < 10; i++)
//		{
//			int arrayX = (i * BoxSize);
//			rendertarget->DrawLine(D2D1::Point2F(x +		   arrayX, y +		     arrayY), D2D1::Point2F(x +		      arrayX, y + BoxSize + arrayY), brush, 3.0f);
//			rendertarget->DrawLine(D2D1::Point2F(x +		   arrayX, y + BoxSize + arrayY), D2D1::Point2F(x + BoxSize + arrayX, y + BoxSize + arrayY), brush, 3.0f);
//			rendertarget->DrawLine(D2D1::Point2F(x + BoxSize + arrayX, y + BoxSize + arrayY), D2D1::Point2F(x + BoxSize + arrayX, y +			arrayY), brush, 3.0f);
//			rendertarget->DrawLine(D2D1::Point2F(x + BoxSize + arrayX, y +		     arrayY), D2D1::Point2F(x +			  arrayX, y +			arrayY), brush, 3.0f);
//
//			// Store the center of the square into the array for later use
//			//centerCoords.push_back(make_pair((float)(x + arrayX/2), (float)(y + arrayY/2)));
//			centerCoords.push_back(make_pair((float)(BoxSize/2 + arrayY), (float)BoxSize/2 + arrayX));
//			
//			k++;
//		}
//
//	}
//	int o = 0;
//}

void Graphics::DrawGrid(float x, float y, float r, float g, float b, float a, vector<pair<float, float>>& centerCoords)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	float lineSpaceX = (RESOLUTION_X - 10) / 10;
	float lineSpaceY = (RESOLUTION_Y - 40) / 10;


		for (float i = 0; i < 11; i++)
		{
			rendertarget->DrawLine(D2D1::Point2F(i*lineSpaceX, 0),
								   D2D1::Point2F(i*lineSpaceX, RESOLUTION_Y), brush, 1.0f);

			rendertarget->DrawLine(D2D1::Point2F(0, i*lineSpaceY),
								   D2D1::Point2F(RESOLUTION_X, i*lineSpaceY), brush, 1.0f);

			// Store the center of each square into the array for our spawning locations
			for (int j = 0; j < 10; j++)
			{
				centerCoords.push_back(make_pair((float)((i*lineSpaceX) + RESOLUTION_X/20),
					(float)((j*lineSpaceY) + RESOLUTION_Y/20)));
			}
		}
}

void Graphics::DrawMenu(float x, float y, float r, float g, float b, float a)
{
	// Get the size of the window
	D2D1_SIZE_F rtSize = rendertarget->GetSize();

	HRESULT res = rendertarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &SolidGrayBrush);
	if (res != S_OK) return;

	// Set the two brush colors
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	SolidGrayBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));

	// DEBUG
	//D2D1_ROUNDED_RECT center = CreateRect(rtSize, 5, 5, 5, 5);
	//rendertarget->DrawRoundedRectangle(center, brush, 5.f);
	// DEBUG

	// Draw outside box
	D2D1_ROUNDED_RECT roundedRect = CreateRect(rtSize, 300, 300, 300, 300);
	// Draw the planet box
	D2D1_ROUNDED_RECT roundedRect2 = CreateRect(rtSize, 270, 250, -30, -30);
	// Draw the info box
	D2D1_ROUNDED_RECT roundedRect3 = CreateRect(rtSize, 270, -70, 270, 30);
	// Draw the Option 1 box
	D2D1_ROUNDED_RECT roundedRect4 = CreateRect(rtSize, 200, -120, 200, 80);
	// Draw the Option 2 box
	D2D1_ROUNDED_RECT roundedRect5 = CreateRect(rtSize, 200, -170, 200, 130);
	// Draw the Option 3 box
	D2D1_ROUNDED_RECT roundedRect6 = CreateRect(rtSize, 200, -220, 200, 180);

	// Render the boxes
	rendertarget->DrawRoundedRectangle(roundedRect, brush, 5.f);
	rendertarget->FillRoundedRectangle(roundedRect, SolidGrayBrush);
	rendertarget->DrawRoundedRectangle(roundedRect2, brush, 5.f);
	rendertarget->DrawRoundedRectangle(roundedRect3, brush, 5.f);
	rendertarget->DrawRoundedRectangle(roundedRect4, brush, 5.f); 
	rendertarget->DrawRoundedRectangle(roundedRect5, brush, 5.f);
	rendertarget->DrawRoundedRectangle(roundedRect6, brush, 5.f);
}

D2D1_ROUNDED_RECT Graphics::CreateRect(D2D1_SIZE_F rtSize, float left, float top, float right, float bottom)
{
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		D2D1::RectF(rtSize.width / 2 - left,
			rtSize.height / 2 - top,
			rtSize.width / 2 + right,
			rtSize.height / 2 + bottom),
		10.f,
		10.f
	);
	return roundedRect;
}

// This method was developed with help from a fellow SET Student - Bence Karner. 
bool Graphics::DrawMenuText(float energy, float science)
{
	static const WCHAR fontName[] = L"Verdana";
	static const FLOAT fonstSize = 25;

		//Create a DirectWrite factory.
		HRESULT operationResult = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown **>(&DWriteFactory)
		);
		if (!operationResult == S_OK) return false;


		//Create a DirectWrite text format object
		operationResult = DWriteFactory->CreateTextFormat(
			fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fonstSize,
			L"",
			&pTextFormat
		);
		if (!operationResult == S_OK) return false;


		//Center the text horizontally and vertically
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		if (!operationResult == S_OK) return false;

		//Retrieve the size of the render target
		D2D1_SIZE_F rtSize = Graphics::GetRenderTarget()->GetSize();
		Graphics::GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());

		operationResult = rendertarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
		if (operationResult != S_OK) return false;


		const WCHAR title[] = L"PLANETARY SCAN";
		// Concat X label and the amount of X together
		const WCHAR energyText[50] = L"Energy:";
		wchar_t energyBuffer[50];
		swprintf_s(energyBuffer, L"%.f", energy);
		wcscat((WCHAR*)energyText, (WCHAR*)energyBuffer);

		const WCHAR scienceText[50] = L"Science: ";
		wchar_t scienceBuffer[50];
		swprintf_s(scienceBuffer, L"%.f", science);
		wcscat((WCHAR*)scienceText, (WCHAR*)scienceBuffer);

		const WCHAR orders[] = L"Orders, Captain?";
		const WCHAR option1[] = L"1. Replenish Energy";
		const WCHAR option2[] = L"2. Gather Science";
		const WCHAR option3[] = L"3. Leave Orbit";

		DrawTextToMenu(rtSize, title, ARRAYSIZE(title), 0, 400, 300, 30);
		DrawTextToMenu(rtSize, energyText, ARRAYSIZE(energyText), 0, 300, 200, 30);
		DrawTextToMenu(rtSize, scienceText, ARRAYSIZE(scienceText), 0, 250, 200, 30);
		DrawTextToMenu(rtSize, orders, ARRAYSIZE(orders), 270, -70, 270, 30);
		DrawTextToMenu(rtSize, option1, ARRAYSIZE(option1), 200, -120, 200, 80);
		DrawTextToMenu(rtSize, option2, ARRAYSIZE(option2), 200, -170, 200, 130);
		DrawTextToMenu(rtSize, option3, ARRAYSIZE(option3), 200, -220, 200, 180);

		if (!operationResult == S_OK) return false;

	return true;
}

void Graphics::DrawTextToMenu(D2D1_SIZE_F rtSize, const wchar_t* string, unsigned int size, float left, float top, float right, float bottom)
{
	//const WCHAR title[] = L"Energy: ";
	int test = sizeof(string)/sizeof(wchar_t);
	Graphics::GetRenderTarget()->DrawText(
		string,
		size - 1,
		pTextFormat,
		D2D1::RectF(rtSize.width / 2 - left, rtSize.height / 2 - top,
			rtSize.width / 2 + right, rtSize.height / 2 + bottom),
		pBlackBrush
	);
}

void Graphics::DrawPlayerStats(float energy, float science)
{
	static const WCHAR fontName[] = L"Verdana";
	static const FLOAT fonstSize = 20;

	//Create a DirectWrite factory.
	HRESULT operationResult = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown **>(&DWriteFactory)
	);
	if (!operationResult == S_OK) return;


	//Create a DirectWrite text format object
	operationResult = DWriteFactory->CreateTextFormat(
		fontName,
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fonstSize,
		L"",
		&pTextFormat
	);
	if (!operationResult == S_OK) return;


	//Center the text horizontally and vertically
	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	if (!operationResult == S_OK) return;

	//Retrieve the size of the render target
	D2D1_SIZE_F rtSize = Graphics::GetRenderTarget()->GetSize();
	Graphics::GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());

	operationResult = rendertarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBlackBrush);
	if (operationResult != S_OK) return;

	// Concat X label and the amount of X together
	const WCHAR energyText[50] = L"Energy:";
	wchar_t energyBuffer[50];
	swprintf_s(energyBuffer, L"%.f", energy);
	wcscat((WCHAR*)energyText, (WCHAR*)energyBuffer);

	const WCHAR scienceText[50] = L"Science: ";
	wchar_t scienceBuffer[50];
	swprintf_s(scienceBuffer, L"%.f", science);
	wcscat((WCHAR*)scienceText, (WCHAR*)scienceBuffer);

	DrawTextToMenu(rtSize, energyText, ARRAYSIZE(energyText), -300, rtSize.height/2, rtSize.width/2, 30);
	DrawTextToMenu(rtSize, scienceText, ARRAYSIZE(scienceText), -300, rtSize.height/2 - 50, 500, 30);

	if (!operationResult == S_OK) return;

}