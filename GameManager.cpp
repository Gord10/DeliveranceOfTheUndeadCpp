#pragma once
#include "GameManager.h"
#include <iostream>
#include <string>
#include "raymath.h"

//Move startValue to target value, using the delta
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
    //Load files
    font = LoadFont("resources//font//alagard.png");

    intro.Init("resources//Story//Intro.txt");
    intro.ReadTexture("resources//Story//Intro.png");

    goodEnding.Init("resources//Story//GoodEnding.txt");
    goodEnding.ReadTexture("resources//Story//GoodEnding.png");

    badEnding.Init("resources//Story//BadEnding.txt");
    badEnding.ReadTexture("resources//Story//BadEnding.png");

    currentStory = &intro;

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

    if (state == IN_GAME)
    {
        float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
        health -= deltaTime * healthDecreaseBySecond; //Decrease the health. Vampire is losing blood continuously

        if (health <= 0)
        {
            ResetGame(); //Game over
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

                if (humanity >= 0.98) //Player became a mortal
                {
                    humanity = 1;
                    currentStory = &goodEnding;
                    state = STORY;
                    currentStory->ReadFile();
                    currentStory->ShowNextLine();
                    return;
                }
            }
        }

        //Check if player is colliding a villager
        for (i = 0; i < villagersAmount; i++)
        {
            Vector2 villagerPos = villagers[i].GetPosition();
            float distanceFromPlayer = Vector2Distance(villagerPos, player.GetPosition());
            float minDistanceToCollect = 40;

            if (distanceFromPlayer <= minDistanceToCollect) //Player is feeding from the villager
            {
                cout << "Feed\n";
                health += healthIncreasePerFeed;
                if (health > 1)
                {
                    health = 1;
                }
                
                audioManager.PlayFeedSound();

                humanity -= humanityLossPerFeed;
                if (humanity <= 0) //Game over. Show bad ending
                {
                    currentStory = &badEnding;
                    state = STORY;
                    currentStory->ReadFile();
                    currentStory->ShowNextLine();
                    return;
                }

                villagers[i].SpawnAtRandomPosition(false, player.GetPosition()); //Spawn the object in a random position distant from the player
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

            if (distanceFromPlayer < minDistanceToGetHarmed) //Cross will harm the player
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

        //Set the camera position
        cameraPos = player.GetPosition();
        cameraPos.x -= GAME_RESOLUTION_WIDTH / 2;
        cameraPos.y -= GAME_RESOLUTION_HEIGHT / 2;
        cameraPos.y -= 50;

        if (isPlayerHarmed && framesPassed % 15 == 0) //Screen shake
        {
            cameraPos.x += GetRandomValue(-20, 20) * 0.1;
            cameraPos.y += GetRandomValue(-20, 20) * 0.1;
        }

        //Tick each objects in list
        list<GameObject*>::iterator it;
        for (it = gameObjects.begin(); it != gameObjects.end(); it++)
        {
            (*it)->Tick(deltaTime);
        }
    }
    else if (state == TITLE)
    {
        player.Tick(deltaTime);
        if(!IsKeyPressed(KEY_F1) && (GetKeyPressed() || GetGamepadButtonPressed() > 0)) //Show the intro if any key is pressed but F1 (which is fullscreen key)
        {
            state = STORY;
            currentStory = &intro;
            currentStory->ReadFile();
            currentStory->ShowNextLine();
        }
    }
    else if (state == STORY)
    {
        if ((GetKeyPressed() || IsGamepadButtonPressed(0, 7) > 0)) //7 is the south button of the right side of the gamepad. A button for Xbox gamepad.
        {
            currentStory->ShowNextLine();
            if (currentStory->isCompleted)
            {
                state = IN_GAME;
                ResetGame(); //Start the game
                cout << "Start game" <<endl;
            }
        }
        
    }
}

void GameManager::Render()
{
    float scale = GetRenderHeight() / GAME_RESOLUTION_HEIGHT;
    
    if (state == IN_GAME)
    {
        //Draw ground
        DrawTextureTiled(groundTex, { 0, 0, (float)groundTex.width, (float)groundTex.height }, { (-2048 - cameraPos.x) * scale, (-2048 - cameraPos.y) * scale, 4096 * scale, 4096 * scale }, { 0, 0 }, 0, scale, WHITE);

        //Draw game limits. They will be red
        DrawLineEx({ (-GAME_MAX_X - cameraPos.x) * scale, (-GAME_MAX_Y - cameraPos.y) * scale }, { (GAME_MAX_X - cameraPos.x) * scale, (-GAME_MAX_Y - cameraPos.y) * scale }, scale, DOTU_RED);
        DrawLineEx({ (-GAME_MAX_X - cameraPos.x) * scale, (GAME_MAX_Y - cameraPos.y) * scale }, { (GAME_MAX_X - cameraPos.x) * scale, (GAME_MAX_Y - cameraPos.y) * scale }, scale, DOTU_RED);
        DrawLineEx({ (-GAME_MAX_X - cameraPos.x) * scale, (-GAME_MAX_Y - cameraPos.y) * scale }, { (-GAME_MAX_X - cameraPos.x) * scale, (GAME_MAX_Y - cameraPos.y) * scale }, scale, DOTU_RED);
        DrawLineEx({ (GAME_MAX_X - cameraPos.x) * scale, (-GAME_MAX_Y - cameraPos.y) * scale }, { (GAME_MAX_X - cameraPos.x) * scale, (GAME_MAX_Y - cameraPos.y) * scale }, scale, DOTU_RED);

        //Sort the game objects according to their Y positions, so we will render them in correct order later
        gameObjects.sort([](const GameObject* A, const GameObject* B) {return A->y < B->y; });

        //Render each objects
        list<GameObject*>::iterator it;
        for (it = gameObjects.begin(); it != gameObjects.end(); it++)
        {
            (*it)->Render(cameraPos);
        }

        RenderUI(scale);
    }
    else if (state == TITLE)
    {
        //Render the title
        const char* titleText = "Deliverance of the Undead";
        float fontSize = 30;
        float textWidth = MeasureTextEx(font, titleText, fontSize, 2).x;
        Vector2 textPos = { (GAME_RESOLUTION_WIDTH - textWidth) * scale * 0.5, (GAME_RESOLUTION_HEIGHT - 100) * scale };
        DrawTextEx(font, titleText, textPos, fontSize * scale, 2 * scale, DOTU_RED);

        //Render the player
        cameraPos = player.GetPosition();
        cameraPos.x -= GAME_RESOLUTION_WIDTH / 2;
        cameraPos.y -= GAME_RESOLUTION_HEIGHT / 2;
        cameraPos.y -= 50;

        player.Render(cameraPos);

        //Render the "Press any key to play" text with blink effect
        if ((int)(timePassed * 2) % 2 == 0)
        {
            const char* pressSpaceText = "Press any key to play";
            fontSize = 20;
            textWidth = MeasureTextEx(font, pressSpaceText, fontSize, 2).x;
            textPos.x = (GAME_RESOLUTION_WIDTH - textWidth) * scale * 0.5;
            textPos.y = (GAME_RESOLUTION_HEIGHT - 65) * scale;

            DrawTextEx(font, pressSpaceText, textPos, fontSize * scale, 2 * scale, WHITE);
        }

        //Render the "F1: Fullscreen" text
        const char* fullScreenText = "F1: Fullscreen";
        fontSize = 20;
        textWidth = MeasureText(fullScreenText, fontSize);
        textPos.x = (GAME_RESOLUTION_WIDTH - textWidth) * 0.5 * scale;
        textPos.y = (GAME_RESOLUTION_HEIGHT - 40) * scale;

        DrawTextEx(font, fullScreenText, textPos, fontSize * scale, 2 * scale, WHITE);
    }
    else if (state == STORY)
    {
        currentStory->Render(font, scale);
    }
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
    audioManager.Unload();

    intro.Unload();
    goodEnding.Unload();
    badEnding.Unload();
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

    healthBarFillRatio = MoveTowards(healthBarFillRatio, health, GetFrameTime()); //We will show the blood level in UI gradually, not instantly
 
    //Draw blood (health) bar
    DrawRectangle(margin, barY *scale, barWidth, barHeight, BLACK);
    DrawRectangle(margin, barY * scale, barWidth * healthBarFillRatio, barHeight, DOTU_RED);

    //Draw "Blood" text on blood bar
    int textWidth = MeasureText("Blood", 10);
    int textX = (GetRenderWidth() - textWidth * scale) / 2;
    DrawTextEx(font, "Blood", {(float) textX, barY * scale}, fontSize * scale, 2, WHITE);

    //Draw humanity bar
    textWidth = MeasureText("Humanity", 10);
    textX = (GetRenderWidth() - textWidth * scale) / 2;
    barY = 36;
    DrawRectangle(margin, barY * scale, barWidth, barHeight, BLACK);

    bool willRenderHumanityBar = true;
    humanityBarFillRatio = MoveTowards(humanityBarFillRatio, humanity, GetFrameTime()); //We will show the humanity level in UI gradually, not instantly

    if (humanityBarFillRatio < humanityLossPerFeed && (int) (timePassed * 3.0) % 2 == 0) //If the player is at the limit of losing their all humanity, we will blink the humanity bar
    {
        willRenderHumanityBar = false;
    }

    if (willRenderHumanityBar)
    {
        //Draw the foreground of humanity bar
        DrawRectangle(margin, barY * scale, barWidth * humanityBarFillRatio, barHeight, DOTU_GREEN);
    }
    
    //Draw the "Humanity" text
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

    //Default values of health and humanity
    health = 1;
    humanity = 0.25;

    healthBarFillRatio = health;
    humanityBarFillRatio = humanity;
}

