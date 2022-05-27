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
	//Render the object considering the resolution and camera position
	DrawTextureEx(texture, { (x - cameraPos.x - (texture.width / 2)) * scale, (y - cameraPos.y - texture.height) * scale}, 0, scale, tintColor);
}

void GameObject::Translate(float xSpeed, float ySpeed) //Move the object by this speed
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
	float x = GetRandomValue(-GAME_MAX_X, GAME_MAX_X);
	float y = GetRandomValue(-GAME_MAX_Y, GAME_MAX_Y);

	if (!isAllowedToSpawnCloseToPlayer)
	{
		Vector2 pos = { x, y };
		float distanceFromPlayer = Vector2Distance(pos, playerPos);
		float minDistanceFromPlayer = 200;
		if (distanceFromPlayer < minDistanceFromPlayer) //Get another random position if the position is too close to player
		{
			SpawnAtRandomPosition(isAllowedToSpawnCloseToPlayer, playerPos);
			return;
		}
	}

	SetPosition(x, y);
}

void GameObject::SetTintColor(Color tintColor)
{
	this->tintColor = tintColor;
}