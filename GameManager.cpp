#include "GameManager.h"
#include <iostream>
#include <string>
#include "raymath.h"
Vector2 cameraPos;
Texture groundTex;

GameManager::GameManager()
{

}

void GameManager::Init()
{
    font = LoadFont("resources//Font//alagard.png");

	player.LoadSprites();
	player.SetPosition(GAME_RESOLUTION_WIDTH / 2, GAME_RESOLUTION_HEIGHT /2);
    gameObjects.push_back(&player);

    groundTex = LoadTexture("resources//Environment//ground.png");

    char fileName[256];
    int i;
    for (i = 0; i < crossAmount; i++)
    {
        snprintf(fileName, 256, "resources//Environment//Cross%d.png", ((i %3) + 1));
        //const char* fileName = "resources//Environment//Cross1.png";
        crosses[i].LoadSprite(fileName);

        float x = GetRandomValue(-70, 70) * 10;
        float y = GetRandomValue(-70, 70) * 10;
        crosses[i].SetPosition(x, y);
        gameObjects.push_back(&crosses[i]);
    }

    snprintf(fileName, 256, "resources//Goblet.png");
    for (i = 0; i < gobletsAmount; i++)
    {
        goblets[i].LoadSprite(fileName);
        float x = GetRandomValue(-50, 50) * 10;
        float y = GetRandomValue(-50, 50) * 10;
        goblets[i].SetPosition(x, y);
        gameObjects.push_back(&goblets[i]);
    }
}

void GameManager::Tick(float deltaTime)
{
    Vector2 playerVelocity = { 0, 0 };
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        playerVelocity.x = -1;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        playerVelocity.x = 1;
    }

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        playerVelocity.y = -1;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        playerVelocity.y = 1;
    }

    playerVelocity = Vector2Normalize(playerVelocity);
    playerVelocity = Vector2Scale(playerVelocity, player.speed);

    player.Translate(playerVelocity.x, playerVelocity.y);

	player.Tick(deltaTime);
    float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
    cameraPos = player.GetPosition();
    cameraPos.x -= GAME_RESOLUTION_WIDTH / 2;
    cameraPos.y -= GAME_RESOLUTION_HEIGHT / 2;
    cameraPos.y -= 50;

    health -= deltaTime * healthDecreaseBySecond;

    //Check if player is colliding a goblet
    int i;
    for (i = 0; i < gobletsAmount; i++)
    {
        if (!goblets[i].isActive)
        {
            continue;
        }
        Vector2 gobletPos = goblets[i].GetPosition();
        float distanceFromPlayer = Vector2Distance(gobletPos, player.GetPosition());
        float minDistanceToCollect = 20;
        if (distanceFromPlayer <= minDistanceToCollect)
        {
            goblets[i].isActive = false;
            humanity += humanityIncreasePerGoblet;

            if (humanity >= 1)
            {
                humanity = 1;
            }
        }
    }
}

void GameManager::Render()
{
    float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
    DrawTextureTiled(groundTex, {0, 0, (float) groundTex.width, (float) groundTex.height}, { (-2048 -cameraPos.x) * scale, (-2048 -cameraPos.y) * scale, 4096 * scale, 4096 * scale}, { 0, 0 }, 0, scale, WHITE);

    gameObjects.sort([](const GameObject* A, const GameObject* B) {return A->y < B->y; });

    list<GameObject*>::iterator it;
    for (it = gameObjects.begin(); it != gameObjects.end(); it++)
    {
       (*it)->Render(cameraPos);
    }

    RenderUI(scale);
}

void GameManager::Unload()
{
    std::list<GameObject*>::iterator it;
    for (it = gameObjects.begin(); it != gameObjects.end(); it++)
    {
        (*it)->Unload();
    }

    UnloadTexture(groundTex);
    UnloadFont(font);
}

void GameManager::RenderUI(float scale)
{
    float barWidth = 160;
    barWidth *= scale;
    float margin = GetRenderWidth() - barWidth;
    margin /= 2;
    float fontSize = 16;
    float barY = 12;
    float barHeight = 16 * scale;
 
    DrawRectangle(margin, barY *scale, barWidth, barHeight, BLACK);
    DrawRectangle(margin, barY * scale, barWidth * health, barHeight, DOTU_RED);

    int textWidth = MeasureText("Blood", 10);
    int textX = (GetRenderWidth() - textWidth * scale) / 2;
    DrawTextEx(font, "Blood", {(float) textX, barY * scale}, fontSize * scale, 2, WHITE);

    textWidth = MeasureText("Humanity", 10);
    textX = (GetRenderWidth() - textWidth * scale) / 2;

    barY = 36;
    DrawRectangle(margin, barY * scale, barWidth, barHeight, BLACK);
    DrawRectangle(margin, barY * scale, barWidth * humanity, barHeight, DOTU_GREEN);
    DrawTextEx(font, "Humanity", { (float)textX, barY * scale }, fontSize * scale, 2, WHITE);
}