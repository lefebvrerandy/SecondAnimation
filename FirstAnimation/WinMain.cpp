#include "Window.h"
#include "Graphics.h"
#include "Level1.h"
#include "GameController.h"
#include "GameDimensions.h"
#include "Sound.h"
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

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

	// Used to force the game to play at 60 fps
	using clock = std::chrono::high_resolution_clock;
	std::chrono::nanoseconds lag(0ns);
	auto time_start = clock::now();
	constexpr std::chrono::nanoseconds timestep(16ms);

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

			// Force updates only at 60 fps
			auto delta_time = clock::now() - time_start;
			time_start = clock::now();
			lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

			// We use the lag variable to handle updates at a constantant rate
			// not a static rate
			while (lag >= timestep)//MS_PER_UPDATE
			{
				lag -= timestep;
				GameController::Update();
			}

			graphics->BeginDraw();
			GameController::Render();
			graphics->EndDraw();

		}
	}

#pragma endregion
	delete graphics;
	return 0;
}

double getCurrentTime()
{
	using namespace std;
	using namespace std::chrono;


	double currentTime = 0;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	//currentTime = t1;

	return currentTime;
}