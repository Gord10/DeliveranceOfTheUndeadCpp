#include "GameManager.h"
#include <iostream>
#include <string>

Vector2 cameraPos;

GameManager::GameManager()
{

}

void GameManager::Init()
{
	player.LoadSprites();
	player.SetPosition(GAME_RESOLUTION_WIDTH / 2, GAME_RESOLUTION_HEIGHT /2);

    int i;
    for (i = 0; i < crossAmount; i++)
    {
        char fileName[256];
        snprintf(fileName, 256, "resources//Environment//Cross%d.png", ((i %3) + 1));
        //const char* fileName = "resources//Environment//Cross1.png";
        crosses[i].LoadSprite(fileName);

        float x = GetRandomValue(-50, 50) * 10;
        float y = GetRandomValue(-50, 50) * 10;
        crosses[i].SetPosition(x, y);
    }
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
    cameraPos = player.GetPosition();
}

void GameManager::Render()
{
    player.Render(cameraPos);

    int i;
    for (i = 0; i < crossAmount; i++)
    {
        crosses[i].Render(cameraPos);
    }
}

void GameManager::Unload()
{
	player.Unload();
}