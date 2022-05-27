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
	bool isHarmed = false;

	float animationCounter = 0;
	int spriteIndex = 0;
	Texture2D idleSpriteSheet;
	Texture2D walkingSpriteSheet;
	int framesAmount = 3; //How many frames are there in the sprite sheet

	void Tick(float deltaTime)
	{
		//Read the input from keyboard
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

		//If we haven't read movement key from keyboard, try reading from the gamepad
		if (velocity.x == 0 && velocity.y == 0)
		{
			if (IsGamepadAvailable(0))
			{
				//Read input from gamepad
				velocity.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
				velocity.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
			}
		}

		velocity = Vector2Normalize(velocity);
		velocity = Vector2Scale(velocity, speed);
		Translate(velocity.x, velocity.y); //Move the vampire

		//Set the sprite index according to the time and how many frames are there in the sprite sheet
		animationCounter += deltaTime;
		float animationTime = 0.125;
		if (animationCounter >= animationTime)
		{
			spriteIndex++;
			spriteIndex %= framesAmount;
			animationCounter = 0;
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
		isWalking = speedX != 0 || speedY != 0; //Vampire will play walking animation if speedX and speedY are not Z (i.e., the vampire is moving)

		//We will flip the sprite later according to speedX's direction
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
		texture = (isWalking) ? walkingSpriteSheet : idleSpriteSheet; //Which sprite sheet will be used?

		Rectangle frameRec = { 0.0f, 0.0f, (float)idleSpriteSheet.width / (float) framesAmount, (float)idleSpriteSheet.height };
		frameRec.x = (float)spriteIndex * ((float)idleSpriteSheet.width / (float)framesAmount);

		Rectangle destRec = { x - cameraPos.x, y - cameraPos.y, texture.width / (float)framesAmount, texture.height};

		//Vampire's feet will be at X and Y positions
		destRec.x -= frameRec.width / 2;
		destRec.y -= texture.height;

		//Make sure the destination rectangle is scaled, so it will look correct when the screen resolution is different
		destRec.x *= scale;
		destRec.y *= scale;
		destRec.width *= scale;
		destRec.height *= scale;
				
		if (isMovingLeft)
		{
			frameRec.width *= -1; //Flip the frame
		}

		DrawTexturePro(texture, frameRec, destRec, { 0, 0 }, 0, tintColor);
	}

	void SetHarmed(bool isHarmed) //Tilt color will be red if the vampire is hurt
	{
		this->isHarmed = isHarmed;
		Color red = DOTU_RED;
		Color white = WHITE;
		tintColor = (isHarmed) ? red : white;
	}
};