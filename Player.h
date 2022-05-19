#pragma once
#include "GameObject.h"
#include <iostream>

using namespace std;

class Player : public GameObject
{
public:

	Texture2D runningSprites[3];
	float animationCounter = 0;
	int spriteIndex = 0;

	void Tick(float deltaTime)
	{
		//cout << deltaTime;
		animationCounter += deltaTime;

		if (animationCounter > 0.125)
		{
			spriteIndex++;
			spriteIndex %= 3;
			animationCounter = 0;
			texture = runningSprites[spriteIndex];
		}
	}

	void LoadSprites()
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			char fileName[2048];
			std::snprintf(fileName, 2048, "resources/alas%d.png", (i + 1));
			cout << fileName;
			runningSprites[i] = LoadTexture(fileName);
		}

		this->texture = runningSprites[0];
	}
};