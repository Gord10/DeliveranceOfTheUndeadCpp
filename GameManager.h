#pragma once
#include "GameObject.h"
#include "Player.h"

//#define GAME_RESOLUTION_WIDTH 640
//#define GAME_RESOLUTION_HEIGHT 360

class GameManager
{
public:
	GameManager();

	float health = 1;
	Font font;
	const static int crossAmount = 50;
	Player player;
	GameObject crosses[crossAmount];
	
	Vector2 cameraPos;

	void Init();
	void Tick(float deltaTime);
	void Unload();
	void Render();

	void RenderUI(float scale);
};


