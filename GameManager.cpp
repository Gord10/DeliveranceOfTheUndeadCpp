#include "GameManager.h"
#include <iostream>
#include <string>
#include "raymath.h"
Vector2 cameraPos;
Texture groundTex;

float humanityBarFillRatio = 0;
float healthBarFillRatio = 1;
Color crossNormalColor = { 89, 86, 82, 255};
int framesPassed = 0;

float MoveTowards(float startValue, float targetValue, float delta)
{
    if (startValue < targetValue)
    {
        startValue += delta;
        if (startValue > targetValue)
        {
            startValue = targetValue;
        }
    }
    else if (startValue > targetValue)
    {
        startValue -= delta;
        if (startValue < targetValue)
        {
            startValue = targetValue;
        }
    }

    return startValue;
}

GameManager::GameManager()
{

}

void GameManager::Init()
{
    font = LoadFont("resources//Font//alagard.png");

	player.LoadSprites();
    gameObjects.push_back(&player);

    groundTex = LoadTexture("resources//Environment//ground.png");

    char fileName[256];
    int i;
    for (i = 0; i < crossAmount; i++)
    {
        snprintf(fileName, 256, "resources//Environment//Cross%d.png", ((i %3) + 1));
        crosses[i].LoadSprite(fileName);
        gameObjects.push_back(&crosses[i]);
    }

    snprintf(fileName, 256, "resources//Goblet.png");
    for (i = 0; i < gobletsAmount; i++)
    {
        goblets[i].LoadSprite(fileName);
        gameObjects.push_back(&goblets[i]);
    }

    for (i = 0; i < villagersAmount; i++)
    {
        snprintf(fileName, 256, "resources//Villager%d.png", ((i % 3) + 1));
        villagers[i].LoadSprite(fileName);
        villagers[i].AssignPlayer(&player);
        gameObjects.push_back(&villagers[i]);
    }

    audioManager.Init();
    ResetGame();
}

void GameManager::Tick(float deltaTime)
{
    timePassed += deltaTime;
    framesPassed++;

    float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;


    health -= deltaTime * healthDecreaseBySecond;

    if (health <= 0)
    {
        ResetGame();
        return;
    }

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
            goblets[i].SpawnAtRandomPosition(false, player.GetPosition());
            humanity += humanityIncreasePerGoblet;
            audioManager.PlayHumanity();
            if (humanity >= 1)
            {
                humanity = 1;
            }
        }
    }

    //Check if player is colliding a villager
    for (i = 0; i < villagersAmount; i++)
    {
        Vector2 villagerPos = villagers[i].GetPosition();
        float distanceFromPlayer = Vector2Distance(villagerPos, player.GetPosition());
        float minDistanceToCollect = 40;

        if (distanceFromPlayer <= minDistanceToCollect)
        {
            cout << "Feed\n";
            health += healthIncreasePerFeed;
            if (health > 1)
            {
                health = 1;
            }

            humanity -= humanityLossPerFeed;
            if (humanity <= 0)
            {
                ResetGame();
                return;
            }
            else
            {
                audioManager.PlayFeedSound();
            }

            villagers[i].SpawnAtRandomPosition(false, player.GetPosition());
        }
    }

    bool isPlayerHarmed = false;

    //Check if player is violating the graveyard boundaries
    if (player.x < -GAME_MAX_X || player.x > GAME_MAX_X || player.y < -GAME_MAX_Y || player.y > GAME_MAX_Y)
    {
        health -= outOfBoundariesHarmPerSecond * deltaTime;
        isPlayerHarmed = true;
    }

    //Check if player is close to a cross
    for (i = 0; i < crossAmount; i++)
    {
        Vector2 crossPos = crosses[i].GetPosition();
        float distanceFromPlayer = Vector2Distance(crossPos, player.GetPosition());
        float minDistanceToGetHarmed = 60;

        if (distanceFromPlayer < minDistanceToGetHarmed)
        {
            health -= crossHarmPerSecond * deltaTime;
            isPlayerHarmed = true;
            crosses[i].SetTintColor(DOTU_RED);
        }
        else
        {
            crosses[i].SetTintColor(crossNormalColor);
        }
    }

    player.SetHarmed(isPlayerHarmed);

    cameraPos = player.GetPosition();
    cameraPos.x -= GAME_RESOLUTION_WIDTH / 2;
    cameraPos.y -= GAME_RESOLUTION_HEIGHT / 2;
    cameraPos.y -= 50;

    if (isPlayerHarmed && framesPassed % 15 == 0)
    {
        cameraPos.x += GetRandomValue(-20, 20) * 0.1;
        cameraPos.y += GetRandomValue(-20, 20) * 0.1;
    }

    list<GameObject*>::iterator it;
    for (it = gameObjects.begin(); it != gameObjects.end(); it++)
    {
        (*it)->Tick(deltaTime);
    }
}

void GameManager::Render()
{
    float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
    DrawTextureTiled(groundTex, {0, 0, (float) groundTex.width, (float) groundTex.height}, { (-2048 -cameraPos.x) * scale, (-2048 -cameraPos.y) * scale, 4096 * scale, 4096 * scale}, { 0, 0 }, 0, scale, WHITE);

    DrawLineEx({ (-GAME_MAX_X - cameraPos.x) * scale, (-GAME_MAX_Y - cameraPos.y) * scale}, { (GAME_MAX_X - cameraPos.x) * scale, (-GAME_MAX_Y -cameraPos.y) * scale}, scale, DOTU_RED);
    DrawLineEx({ (-GAME_MAX_X - cameraPos.x) * scale, (GAME_MAX_Y - cameraPos.y) * scale }, { (GAME_MAX_X - cameraPos.x) * scale, (GAME_MAX_Y - cameraPos.y) * scale }, scale, DOTU_RED);
    DrawLineEx({ (-GAME_MAX_X - cameraPos.x) * scale, (-GAME_MAX_Y - cameraPos.y) * scale }, { (-GAME_MAX_X - cameraPos.x) * scale, (GAME_MAX_Y - cameraPos.y) * scale }, scale, DOTU_RED);
    DrawLineEx({ (GAME_MAX_X - cameraPos.x) * scale, (-GAME_MAX_Y - cameraPos.y) * scale }, { (GAME_MAX_X - cameraPos.x) * scale, (GAME_MAX_Y - cameraPos.y) * scale }, scale, DOTU_RED);

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

    healthBarFillRatio = MoveTowards(healthBarFillRatio, health, GetFrameTime());
 
    DrawRectangle(margin, barY *scale, barWidth, barHeight, BLACK);
    DrawRectangle(margin, barY * scale, barWidth * healthBarFillRatio, barHeight, DOTU_RED);

    int textWidth = MeasureText("Blood", 10);
    int textX = (GetRenderWidth() - textWidth * scale) / 2;
    DrawTextEx(font, "Blood", {(float) textX, barY * scale}, fontSize * scale, 2, WHITE);

    textWidth = MeasureText("Humanity", 10);
    textX = (GetRenderWidth() - textWidth * scale) / 2;
    barY = 36;
    DrawRectangle(margin, barY * scale, barWidth, barHeight, BLACK);

    bool willRenderHumanityBar = true;
    humanityBarFillRatio = MoveTowards(humanityBarFillRatio, humanity, GetFrameTime());

    if (humanityBarFillRatio < humanityLossPerFeed && (int) (timePassed * 3.0) % 2 == 0)
    {
        willRenderHumanityBar = false;
    }


    if (willRenderHumanityBar)
    {
        DrawRectangle(margin, barY * scale, barWidth * humanityBarFillRatio, barHeight, DOTU_GREEN);
    }
    
    DrawTextEx(font, "Humanity", { (float)textX, barY * scale }, fontSize * scale, 2, WHITE);
}

void GameManager::ResetGame()
{
    player.SetPosition(GAME_RESOLUTION_WIDTH / 2, GAME_RESOLUTION_HEIGHT / 2);
    Vector2 playerPos = player.GetPosition();

    int i;
    for (i = 0; i < crossAmount; i++)
    {
        crosses[i].SpawnAtRandomPosition(false, playerPos);
    }

    for (i = 0; i < gobletsAmount; i++)
    {
        goblets[i].SpawnAtRandomPosition(false, playerPos);
    }

    for (i = 0; i < villagersAmount; i++)
    {
        villagers[i].SpawnAtRandomPosition(false, playerPos);
    }

    health = 1;
    humanity = 0.25;
}

