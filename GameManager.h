#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Villager.h"
#include "AudioManager.h"
#include "Story.h"
#include <list>
using namespace std;

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
	float crossHarmPerSecond = 0.04;
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

	Story intro, goodEnding, badEnding;

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

	Vector2 cameraPos;
	Texture groundTex;

	Story* currentStory;

	list<GameObject*> gameObjects;
	void ResetGame();
};


