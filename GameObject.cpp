#include "raylib.h"
#include "GameManager.h"
#include "GameObject.h"
#include <iostream>

using namespace std;

void GameObject::Render(Vector2 cameraPos)
{
	float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
	DrawTextureEx(texture, { (x - cameraPos.x) * scale, (y - cameraPos.y) * scale}, 0, scale, WHITE);
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