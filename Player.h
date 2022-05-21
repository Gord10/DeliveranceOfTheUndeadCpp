#pragma once
#include "Definitions.h"
#include "raymath.h"
//#include "GameObject.h"
//#include "GameManager.h"
#include <iostream>

using namespace std;

class Player : public GameObject
{
public:
	float speed = 175;
	bool isWalking = false;
	bool isMovingLeft = false;

	float animationCounter = 0;
	int spriteIndex = 0;
	Texture2D idleSpriteSheet;
	Texture2D walkingSpriteSheet;

	void Tick(float deltaTime)
	{
		Vector2 velocity = { 0, 0 };
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
		{
			velocity.x = -1;
		}
		else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		{
			velocity.x = 1;
		}

		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
		{
			velocity.y = -1;
		}
		else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
		{
			velocity.y = 1;
		}

		if (velocity.x == 0 && velocity.y == 0)
		{
			if (IsGamepadAvailable(0))
			{
				velocity.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
				velocity.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
			}
		}

		velocity = Vector2Normalize(velocity);
		velocity = Vector2Scale(velocity, speed);

		Translate(velocity.x, velocity.y);

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

	void Render(Vector2 cameraPos)
	{
		float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
		texture = (isWalking) ? walkingSpriteSheet : idleSpriteSheet;

		Rectangle frameRec = { 0.0f, 0.0f, (float)idleSpriteSheet.width / 3.0, (float)idleSpriteSheet.height };
		frameRec.x = (float)spriteIndex * ((float)idleSpriteSheet.width / 3.0);

		Rectangle destRec = { x - cameraPos.x, y - cameraPos.y, texture.width / 3.0, texture.height };


		destRec.x -= frameRec.width / 2;
		destRec.y -= texture.height;


		destRec.x *= scale;
		destRec.y *= scale;

		/*destRec.x = (int)(GAME_RESOLUTION_WIDTH - texture.width) / 2;
		destRec.y = (int) (GAME_RESOLUTION_HEIGHT - texture.height) / 2;*/
		
		
		destRec.width *= scale;
		destRec.height *= scale;
				
		if (isMovingLeft)
		{
			frameRec.width *= -1;
		}

		//texture = (isWalking) ? walkingSpriteSheet : idleSpriteSheet;

		DrawTexturePro(texture, frameRec, destRec, { 0, 0 }, 0, WHITE);
		//DrawPixel(GetRenderWidth() / 2, GetRenderHeight() / 2, RED);
	}
};