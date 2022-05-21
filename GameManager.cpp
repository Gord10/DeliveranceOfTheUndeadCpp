#include "GameManager.h"
#include <iostream>
#include <string>

Vector2 cameraPos;
Texture groundTex;

GameManager::GameManager()
{

}

void GameManager::Init()
{
	player.LoadSprites();
	player.SetPosition(GAME_RESOLUTION_WIDTH / 2, GAME_RESOLUTION_HEIGHT /2);
    groundTex = LoadTexture("resources//Environment//ground.png");

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
    float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
    cameraPos = player.GetPosition();
    cameraPos.x -= GAME_RESOLUTION_WIDTH / 2;
    cameraPos.y -= GAME_RESOLUTION_HEIGHT / 2;
    cameraPos.y -= 50;
}

void GameManager::Render()
{
    float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
    DrawTextureTiled(groundTex, {0, 0, (float) groundTex.width, (float) groundTex.height}, { (-2048 -cameraPos.x) * scale, (-2048 -cameraPos.y) * scale, 4096 * scale, 4096 * scale}, { 0, 0 }, 0, scale, WHITE);

    int i;
    for (i = 0; i < crossAmount; i++)
    {
        crosses[i].Render(cameraPos);
    }

    player.Render(cameraPos);
}

void GameManager::Unload()
{
	player.Unload();

    int i;
    for (i = 0; i < crossAmount; i++)
    {
        crosses[i].Unload();
    }

    UnloadTexture(groundTex);
}