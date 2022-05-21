#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Villager.h"
#include <list>
using namespace std;
//#define GAME_RESOLUTION_WIDTH 640
//#define GAME_RESOLUTION_HEIGHT 360

class GameManager
{
public:
	GameManager();

	float health = 1;
	float healthDecreaseBySecond = 0.03;
	float humanity = 0.5;
	float humanityIncreasePerGoblet = 0.1;

	Font font;
	const static int crossAmount = 50;
	const static int gobletsAmount = 7;
	const static int villagersAmount = 7;
	Player player;

	GameObject crosses[crossAmount];
	GameObject goblets[gobletsAmount];
	Villager villagers[villagersAmount];

	Vector2 cameraPos;

	void Init();
	void Tick(float deltaTime);
	void Unload();
	void Render();

	void RenderUI(float scale);
private:
	list<GameObject*> gameObjects;

};


