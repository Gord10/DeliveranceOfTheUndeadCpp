#include "raylib.h"
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include "Player.h"
using namespace std;
Font font;

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
    const int screenWidth = 320;
    const int screenHeight = 180;
    int i;
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    Player player(100, 100);

    font = LoadFont("resources/font/alagard.png");
    
    /*
    Texture2D textures[3];

    int i;
    for (i = 0; i < 3; i++)
    {
        char fileName[2048];
        std::snprintf(fileName, 1024, "resources/alas%d.png", (i + 1));
        cout << fileName;
        textures[i] = LoadTexture(fileName);
    }
    */
    int frameCounter = 0;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_F1))
        {
            ChangeFullscreen(screenWidth, screenHeight);
        }

        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        Color backgroundColor = { 16, 16, 16 };
        ClearBackground(backgroundColor);

        float playerSpeed = 40;
        Vector2 playerVelocity = { 0, 0 };
        if (IsKeyDown(KEY_LEFT))
        {
            playerVelocity.x = -playerSpeed;
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            playerVelocity.x = playerSpeed;
        }

        if (IsKeyDown(KEY_UP))
        {
            playerVelocity.y = -playerSpeed;
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            playerVelocity.y = playerSpeed;
        }

        player.Translate(playerVelocity.x, playerVelocity.y);
        
        player.Render();
        player.Tick(GetFrameTime());


        /*
        Vector2 position;
        string title = "Deliverance of the Undead";
        float fontSize = 20;

        position.x = (screenWidth - (title.length() * fontSize) * 0.5) / 2;
        position.y = screenHeight - 30;
        DrawTextEx(font, title.c_str(), position, fontSize, 1, RED);

        int textureIndex = frameCounter / 15;
        textureIndex %= 3;
        Texture texture = textures[textureIndex];

        position.x = (screenWidth - textures[0].width) / 2;
        position.y = (screenHeight - textures[0].height) / 2;
        DrawTextureEx(texture, position, 0, 1.0, WHITE);

        //DrawTexture(texture, screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2, WHITE);
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        */
        EndDrawing();
        frameCounter++;
        //----------------------------------------------------------------------------------
    }

    
    UnloadFont(font);
    player.Unload();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

