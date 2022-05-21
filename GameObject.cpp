#include "raylib.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Villager.h"
#include "Definitions.h"
#include "raymath.h"
#include <iostream>

using namespace std;

void GameObject::Render(Vector2 cameraPos)
{
	if (!isActive)
	{
		return;
	}

	float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
	DrawTextureEx(texture, { (x - cameraPos.x - (texture.width / 2)) * scale, (y - cameraPos.y - texture.height) * scale}, 0, scale, WHITE);
}

void GameObject::Translate(float xSpeed, float ySpeed)
{
	this->x += xSpeed *GetFrameTime();
	this->y += ySpeed *GetFrameTime();
}

void GameObject::Unload()
{
	UnloadTexture(texture);
}

void GameObject::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void GameObject::LoadSprite(const char* fileName)
{
	texture = LoadTexture(fileName);
}

Vector2 GameObject::GetPosition()
{
	return { x, y };
}

Vector2 GameObject::GetCenterPosition(float scale)
{
	Vector2 pos = GetPosition();
	pos.x += (texture.width) / 2;
	pos.y += (texture.height) / 2;
	return pos;
}

void GameObject::Tick(float deltaTime)
{

}

void GameObject::SpawnAtRandomPosition(bool isAllowedToSpawnCloseToPlayer, Vector2 playerPos)
{
	float x = GetRandomValue(-GAME_MAX_X / 10, GAME_MAX_X / 10) * 10;
	float y = GetRandomValue(-GAME_MAX_Y / 10, GAME_MAX_Y / 10) * 10;

	if (!isAllowedToSpawnCloseToPlayer)
	{
		Vector2 pos = { x, y };
		float distanceFromPlayer = Vector2Distance(pos, playerPos);
		if (distanceFromPlayer < 200)
		{
			SpawnAtRandomPosition(isAllowedToSpawnCloseToPlayer, playerPos);
			return;
		}
	}

	SetPosition(x, y);
}