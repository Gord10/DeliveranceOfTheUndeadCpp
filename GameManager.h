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

	float health = 1; //In other words, "blood"
	float healthDecreaseBySecond = 0.02; //How fast the vampire will lose blood
	float humanity = 0.5;
	float humanityIncreasePerGoblet = 0.075; //How much humanity the vampire will gain per goblet
	float healthIncreasePerFeed = 0.25; //How much health (blood) the vampire will gain per villager
	float humanityLossPerFeed = 0.15; //How much humanity the vampire will lose when he feeds from a villager
	float crossHarmPerSecond = 0.04; //Blood lose speed per cross when the vampire gets too close to the cross
	float outOfBoundariesHarmPerSecond = 0.1f; //Vampire will lose blood by this speed when he is out of graveyard boundaries 
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

	//GUI variables
	float humanityBarFillRatio = 0;
	float healthBarFillRatio = 1;

	Color crossNormalColor = { 89, 86, 82, 255 }; //Tint color for crosses when we indicate that they harm the player
	int framesPassed = 0;

	Vector2 cameraPos;
	Texture groundTex;

	Story* currentStory;

	list<GameObject*> gameObjects;
	void ResetGame();
};


