#pragma once
#include "GameObject.h"
#include "Player.h"

//#define GAME_RESOLUTION_WIDTH 640
//#define GAME_RESOLUTION_HEIGHT 360

class GameManager
{
public:
	//static const int resolutionWidth = 640;
	//static const int resolutionHeight = 360;
	
	Player player;
	
	GameManager();

	void Init();
	void Tick(float deltaTime);
	void Unload();
	void Render();
};


