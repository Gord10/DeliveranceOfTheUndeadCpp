#include "raylib.h"
#include "GameObject.h"
#include <iostream>

using namespace std;

GameObject::GameObject(float x, float y)
{
	this->x = x;
	this->y = y;
	this->texture = LoadTexture("resources//Lion/lion_running1.png");
}

GameObject::GameObject()
{
	this->x = 0;
	this->y = 0;
}

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