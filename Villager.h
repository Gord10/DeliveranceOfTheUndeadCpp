#pragma once
#include "GameObject.h"
#include "raymath.h"
#include "GameManager.h"
#include "Definitions.h"

class Villager : public GameObject
{
	Player* player;
	//GameManager* gameManager;
	float speed = 100;
	bool isWalking = false;
	bool isMovingLeft = false;

	float animationCounter = 0;
	int spriteIndex = 0;

	Texture2D runningSpriteSheet;

public:

	void AssignPlayer(Player * player)
	{
		this->player = player;
	}

	/*
	void GameManager(GameManager * gameManager)
	{
		this->gameManager = gameManager;
	}*/

	void Tick(float deltaTime)
	{
		animationCounter += deltaTime;

		if (animationCounter > 0.100)
		{
			spriteIndex++;
			spriteIndex %= 4;
			animationCounter = 0;
		}

		RunFromPlayer(player->GetPosition());

		if (x < -GAME_MAX_X || x > GAME_MAX_X || y < -GAME_MAX_Y || y > GAME_MAX_Y)
		{
			SpawnAtRandomPosition();
		}
	}
	
	void LoadSprite(char *fileName)
	{
		runningSpriteSheet = LoadTexture(fileName);
	}

	void Render(Vector2 cameraPos)
	{
		float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
		texture = runningSpriteSheet;

		Rectangle frameRec = { 0.0f, 0.0f, (float)runningSpriteSheet.width / 4.0, (float)runningSpriteSheet.height };
		frameRec.x = (float)spriteIndex * ((float)runningSpriteSheet.width / 4.0);

		Rectangle destRec = { x - cameraPos.x, y - cameraPos.y, texture.width / 4.0, texture.height };


		destRec.x -= frameRec.width / 2;
		destRec.y -= texture.height;


		destRec.x *= scale;
		destRec.y *= scale;

		destRec.width *= scale;
		destRec.height *= scale;

		if (isMovingLeft)
		{
			frameRec.width *= -1;
		}

		DrawTexturePro(texture, frameRec, destRec, { 0, 0 }, 0, WHITE);
	}

	void RunFromPlayer(Vector2 playerPos)
	{
		Vector2 direction = Vector2Subtract(GetPosition(), playerPos);
		direction = Vector2Normalize(direction);
		direction = Vector2Scale(direction, speed);

		isMovingLeft = direction.x < 0;
		Translate(direction.x, direction.y);
	}
};