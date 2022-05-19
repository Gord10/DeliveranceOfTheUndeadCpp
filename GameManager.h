#pragma once
#include "GameObject.h"
#include "Player.h"

class GameManager
{
public:
	Player player;

	GameManager();

	void Init();
	void Tick(float deltaTime);
	void Unload();
};


