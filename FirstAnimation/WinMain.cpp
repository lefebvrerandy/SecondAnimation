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
	ShowWindow(windowhandle, nCmdShow);
	GameController::LoadInitialLevel(new Level1());

#pragma region GameLoop

	MSG message;
	message.message = WM_NULL;
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&message);
		else
		{
			//Update Routine... we've moved the code for handling updates to GameController
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