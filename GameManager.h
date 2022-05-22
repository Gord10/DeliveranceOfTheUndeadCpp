#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Villager.h"
#include "AudioManager.h"
#include "Story.h"
#include <list>
using namespace std;
//#define GAME_RESOLUTION_WIDTH 640
//#define GAME_RESOLUTION_HEIGHT 360

class GameManager
{
public:
	GameManager();

	enum State
	{
		TITLE = 0,
		IN_GAME,
		STORY
	};

	State state = TITLE;

	float health = 1;
	float healthDecreaseBySecond = 0.02;
	float humanity = 0.5;
	float humanityIncreasePerGoblet = 0.075;
	float healthIncreasePerFeed = 0.25;
	float humanityLossPerFeed = 0.15;
	float crossHarmPerSecond = 0.03;
	float outOfBoundariesHarmPerSecond = 0.1f;
	float timePassed = 0;

	Font font;
	const static int crossAmount = 50;
	const static int gobletsAmount = 7;
	const static int villagersAmount = 7;
	Player player;

	GameObject crosses[crossAmount];
	GameObject goblets[gobletsAmount];
	Villager villagers[villagersAmount];
	AudioManager audioManager;

	Vector2 cameraPos;

	Story intro;

	void Init();
	void Tick(float deltaTime);
	void Unload();
	void Render();

	void RenderUI(float scale);
private:

	float humanityBarFillRatio = 0;
	float healthBarFillRatio = 1;
	Color crossNormalColor = { 89, 86, 82, 255 };
	int framesPassed = 0;

	list<GameObject*> gameObjects;
	void ResetGame();
};


