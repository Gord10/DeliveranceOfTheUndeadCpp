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
    float playerSpeed = 40;
    Vector2 playerVelocity = { 0, 0 };
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        playerVelocity.x = -playerSpeed;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        playerVelocity.x = playerSpeed;
    }

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        playerVelocity.y = -playerSpeed;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        playerVelocity.y = playerSpeed;
    }

    player.Translate(playerVelocity.x, playerVelocity.y);

	player.Tick(deltaTime);
	player.Render();
}

void GameManager::Unload()
{
	player.Unload();
}