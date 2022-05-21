#include "raylib.h"
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include "GameManager.h"
#include "Player.h"
#include "Definitions.h"

using namespace std;

GameManager gameManager;
Font font;
Color BACKGROUND_COLOR = {16, 16, 16};

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
    const int screenWidth = GAME_RESOLUTION_WIDTH;
    const int screenHeight = GAME_RESOLUTION_HEIGHT;

    int i;
    SetConfigFlags(FLAG_VSYNC_HINT);
    //SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    //Player player(100, 100);

    font = LoadFont("resources/font/alagard.png");
    gameManager.Init();

    bool willQuit = false;
    while (!willQuit)    // Detect window close button or ESC key
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

    UnloadFont(font);
    gameManager.Unload();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

