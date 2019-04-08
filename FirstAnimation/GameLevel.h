#pragma once

#include "Graphics.h"
#include "SpriteSheet.h"
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")

class GameLevel
{
protected:
	static Graphics* gfx;

public:
	//GameLevel(GameLevel* &second)
	//{
	//	this->gfx = second->gfx;
	//}
	static void Init(Graphics* graphics)
	{
		gfx = graphics;
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};