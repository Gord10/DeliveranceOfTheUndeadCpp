#pragma once
#include "raylib.h"
//#include "GameManager.h"

extern Vector2 cameraPos;


class GameObject
{
protected: 
	
	Texture2D texture;
	Color tintColor = WHITE;
	
public: 
	bool isActive = true;
	float x = 0, y = 0;
	

	void LoadSprites();
	virtual void LoadSprite(const char * fileName);
	virtual void SetPosition(float x, float y);
	virtual void Render(Vector2 cameraPos);
	virtual void Translate(float speedX, float speedY);
	virtual void Tick(float deltaTime);
	virtual void Unload();
	void SpawnAtRandomPosition(bool isAllowedToSpawnCloseToPlayer, Vector2 playerPos);

	Vector2 GetPosition();
	Vector2 GetCenterPosition(float scale);
	void SetTintColor(Color tintColor);
};