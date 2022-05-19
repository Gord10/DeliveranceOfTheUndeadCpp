#include "raylib.h"
#include "GameObject.h"
#include <iostream>

using namespace std;

void GameObject::Render()
{
	float scale = GetRenderHeight() / 180;
	DrawTextureEx(texture, {(x - (texture.width /2)) * scale, (y - texture.height) * scale}, 0, scale, WHITE);
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


void GameObject::Tick(float deltaTime)
{

}

void GameObject::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}