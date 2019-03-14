#include "Window.h"
#include "Graphics.h"
#include "Level1.h"
#include "GameController.h"
#include "GameDimensions.h"

Graphics* graphics;


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	// Proper way
	//Window* wnd = new Window(800, 400, "Donkey");

	// Video way
	Window wnd(RESOLUTION_X, RESOLUTION_Y, WINDOW_NAME);
	HWND windowhandle = wnd.GetWindowsHandle();

	graphics = new Graphics();
	if (!graphics->Init(windowhandle))
	{
		delete graphics;
		return -1;
	}

	GameLevel::Init(graphics);
	GameController::LoadInitialLevel(new Level1());
	//GameController::LoadInitialLevel(new PlanetMenu());
#pragma region GameLoop

	MSG message;
	message.message = WM_NULL;
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&message);
		else
		{
			// Check if the player is in a level
			if (dynamic_cast<Level1*>(GameController::GetCurrentLevel()))
			{
				// Check to see if the user has clicked anywhere on screen.
				// If so, we need to send the coordinates to the GameController 
				// so that it can move the player ship
				if (message.message == WM_LBUTTONDOWN)
				{
					POINTS coord = MAKEPOINTS(message.lParam);
					GameController::ProcessPlayerCoordinates(coord);
				}
			}
			// Check if the player is in the planet menu
			if (dynamic_cast<PlanetMenu*>(GameController::GetCurrentLevel()))
			{
				if (message.message == WM_KEYDOWN)
				{
					GameController::ProcessSelection(message.wParam);
				}
			}
			GameController::Update();

			graphics->BeginDraw();
			GameController::Render();
			graphics->EndDraw();

		}
	}

#pragma endregion
	delete graphics;
	return 0;
}