#pragma once
#include "raylib.h"

class GameObject
{
protected: 
	float x = 0, y = 0;
	Texture2D texture;
	
public: 

	void LoadSprites();
	void SetPosition(float x, float y);
	void Render();
	void Translate(float speedX, float speedY);
	void Tick(float deltaTime);
	virtual void Unload();
};