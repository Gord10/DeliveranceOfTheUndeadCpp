#pragma once
#include "GameObject.h"
#include "GameManager.h"
#include <iostream>

using namespace std;

class Player : public GameObject
{
public:
	float speed = 90;
	bool isWalking = false;
	bool isMovingLeft = false;

	float animationCounter = 0;
	int spriteIndex = 0;
	Texture2D idleSpriteSheet;
	Texture2D walkingSpriteSheet;

	void Tick(float deltaTime)
	{
		animationCounter += deltaTime;

		if (animationCounter > 0.125)
		{
			spriteIndex++;
			spriteIndex %= 3;
			animationCounter = 0;
			//texture = (isWalking) ? runningSpritesRight[spriteIndex] : idleSpritesRight[spriteIndex];
		}
	}

	void LoadSprites()
	{
		idleSpriteSheet = LoadTexture("resources//AlterIdle.png");
		walkingSpriteSheet = LoadTexture("resources//AlterWalking.png");
	}

	void Unload()
	{
		int i;
		UnloadTexture(idleSpriteSheet);
		UnloadTexture(walkingSpriteSheet);
	}

	void Translate(float speedX, float speedY)
	{
		GameObject::Translate(speedX, speedY);
		isWalking = speedX != 0 || speedY != 0;

		if (speedX < 0)
		{
			isMovingLeft = true;
		}
		else if (speedX > 0)
		{
			isMovingLeft = false;
		}
	}

	void Render()
	{
		float scale = GetRenderHeight() / 360;

		Rectangle frameRec = { 0.0f, 0.0f, (float)idleSpriteSheet.width / 3.0, (float)idleSpriteSheet.height };
		frameRec.x = (float)spriteIndex * ((float)idleSpriteSheet.width / 3.0);

		Rectangle destRec = { x, y, texture.width / 3.0, texture.height };
		destRec.x *= scale;
		destRec.y *= scale;
		destRec.width *= scale;
		destRec.height *= scale;

		if (isMovingLeft)
		{
			frameRec.width *= -1;
		}

		texture = (isWalking) ? walkingSpriteSheet : idleSpriteSheet;

		DrawTexturePro(texture, frameRec, destRec, { 0, 0 }, 0, WHITE);
	}
};