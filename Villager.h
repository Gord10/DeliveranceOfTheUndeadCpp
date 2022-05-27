#pragma once
#include "GameObject.h"
#include "raymath.h"
#include "GameManager.h"
#include "Definitions.h"

class Villager : public GameObject
{
	Player* player;
	//GameManager* gameManager;
	float speed = 70;
	bool isWalking = false;
	bool isMovingLeft = false;

	float animationCounter = 0;
	int spriteIndex = 0;
	int framesAmount = 4;
	Texture2D runningSpriteSheet;

public:

	void AssignPlayer(Player * player)
	{
		this->player = player;
	}

	void Tick(float deltaTime)
	{
		//We will change sprite index according to the time, so we will be able to play a running animation
		animationCounter += deltaTime;
		float animationTime = 0.075;

		if (animationCounter >= animationTime)
		{
			spriteIndex++;
			spriteIndex %= framesAmount;
			animationCounter = 0;
		}

		RunFromPlayer(player->GetPosition());

		//If the villager is not within the graveyard, he is saved. 
		if (x < -GAME_MAX_X || x > GAME_MAX_X || y < -GAME_MAX_Y || y > GAME_MAX_Y)
		{
			SpawnAtRandomPosition(false, player->GetPosition()); //We will spawn this object in another position in game area(i.e., graveyard). He won't be close to the player, because we don't want the player to accidentally lose humanity
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

		//Get the frame from the sprite sheet
		Rectangle frameRec = { 0.0f, 0.0f, (float)runningSpriteSheet.width / (float) framesAmount, (float)runningSpriteSheet.height };
		frameRec.x = (float)spriteIndex * ((float)runningSpriteSheet.width / (float) framesAmount);

		Rectangle destRec = { x - cameraPos.x, y - cameraPos.y, texture.width / (float) framesAmount, texture.height };

		//Let the villager's feet be at X and Y
		destRec.x -= frameRec.width / 2;
		destRec.y -= texture.height;

		//Make sure the destination rectangle is scaled, so it will look correct when the screen resolution is different
		destRec.x *= scale;
		destRec.y *= scale;

		destRec.width *= scale;
		destRec.height *= scale;

		if (isMovingLeft)
		{
			frameRec.width *= -1; //Flip the texture if the villager is running left
		}

		DrawTexturePro(texture, frameRec, destRec, { 0, 0 }, 0, WHITE);
	}

	void RunFromPlayer(Vector2 playerPos)
	{
		//Calculate the running direction. Run to the opposite direction of the vampire
		Vector2 direction = Vector2Subtract(GetPosition(), playerPos);
		direction = Vector2Normalize(direction);
		direction = Vector2Scale(direction, speed);

		isMovingLeft = direction.x < 0; //We will flip the texture if the villager is moving left
		Translate(direction.x, direction.y); //Move the villager
	}
};