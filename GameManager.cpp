#include "GameManager.h"

GameManager::GameManager()
{

}

void GameManager::Init()
{
	player.LoadSprites();
	player.SetPosition(160, 160);
}

void GameManager::Tick(float deltaTime)
{
    Vector2 playerVelocity = { 0, 0 };
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        playerVelocity.x = -player.speed;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        playerVelocity.x = player.speed;
    }

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        playerVelocity.y = -player.speed;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        playerVelocity.y = player.speed;
    }

    player.Translate(playerVelocity.x, playerVelocity.y);

	player.Tick(deltaTime);
}

void GameManager::Render()
{
    player.Render();
}

void GameManager::Unload()
{
	player.Unload();
}