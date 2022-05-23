#include "raylib.h"
#include <iostream>
#include <string>
#include <string_view>
#include "GameManager.h"
#include "Player.h"
#include "Definitions.h"

using namespace std;

GameManager gameManager;
Color BACKGROUND_COLOR = {32, 32, 32};

void ChangeFullscreen(int screenWidth, int screenHeight)
{
    // see what display we are on right now
    int display = GetCurrentMonitor();
    ToggleFullscreen();

    if (!IsWindowFullscreen())
    {
        // if we are full screen, then go back to the windowed size
        SetWindowSize(screenWidth, screenHeight);
    }
    else
    {
        // if we are not full screen, set the window size to match the monitor we are on
        SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
    }
}

int main(void)
{
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    SetConfigFlags(FLAG_VSYNC_HINT);
    //SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "Deliverance of the Undead");
    gameManager.Init();

    bool willQuit = false;
    while (!WindowShouldClose() && !willQuit)    // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_F1))
        {
            ChangeFullscreen(screenWidth, screenHeight);
        }

        if (IsGamepadButtonDown(0, 15) || IsKeyPressed(KEY_ESCAPE))
        {
            willQuit = true;
        }

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        gameManager.Tick(GetFrameTime());
        gameManager.Render();

        EndDrawing();
    }
    
    gameManager.Unload();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

