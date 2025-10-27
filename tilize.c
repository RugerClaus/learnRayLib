#include <raylib.h>

void DrawTiles(int *rows, int *cols, int *ts)
{
    for(int row = 0; row < *rows; row++)
    {
        for(int col = 0; col < *cols; col++)
        {
            int x = col * *ts;
            int y = row * *ts;
            DrawRectangleLines(x,y,*ts,*ts,LIGHTGRAY);

        }
    }
}

int main(void)
{
    // Initialize window
    const int screenWidth = 768;
    const int screenHeight = 768;

    //tiles
    int tileSize = 32;
    int rows = screenHeight / tileSize;
    int cols = screenWidth / tileSize;

    InitWindow(screenWidth, screenHeight, "Hello Raylib!");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTiles(&rows,&cols,&tileSize);
        DrawFPS(725,5);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

