#pragma once
#include "raylib.h"
#include "GameManager.h"

extern Vector2 cameraPos;


class GameObject
{
protected: 
	float x = 0, y = 0;
	Texture2D texture;
	
public: 
	bool isActive = true;

	void LoadSprites();
	void LoadSprite(const char * fileName);
	virtual void SetPosition(float x, float y);
	void Render(Vector2 cameraPos);
	virtual void Translate(float speedX, float speedY);
	void Tick(float deltaTime);
	virtual void Unload();
	Vector2 GetPosition();
	Vector2 GetCenterPosition(float scale);
};