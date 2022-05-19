#include "raylib.h"
#include "GameManager.h"
#include "GameObject.h"
#include <iostream>

using namespace std;

void GameObject::Render()
{
	float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
	//DrawTextureEx(texture, { x, y }, 0, scale, WHITE);
}

void GameObject::Translate(float xSpeed, float ySpeed)
{
	this->x += xSpeed * GetFrameTime();
	this->y += ySpeed * GetFrameTime();
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