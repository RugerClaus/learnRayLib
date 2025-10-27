#include <raylib.h>

typedef struct Player 
{
    Vector2 position;
    float speed;
} Player;

void DrawTiles(int startRow, int endRow, int startCol, int endCol, int tileSize)
{
    for(int row = startRow; row <= endRow; row++)
    {
        for(int col = startCol; col <= endCol; col++)
        {
            int x = col * tileSize;
            int y = row * tileSize;
            DrawRectangleLines(x, y, tileSize, tileSize, LIGHTGRAY);
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

    //setup camera
    Camera2D camera = { 0 };
    
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f}; //centering the camera i'll play with these values to figure out whats what
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Player player = { .position = {screenWidth / 2.0f, screenHeight / 2.0f}, .speed = 150.0f};

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (IsKeyDown(KEY_RIGHT)) player.position.x += player.speed * dt;
        if (IsKeyDown(KEY_LEFT)) player.position.x -= player.speed * dt;
        if (IsKeyDown(KEY_DOWN)) player.position.y += player.speed * dt;
        if (IsKeyDown(KEY_UP)) player.position.y -= player.speed * dt;

        camera.target = player.position;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode2D(camera);

        int startRow = (int)(camera.target.y / tileSize) - (screenHeight/tileSize / 2);
        int endRow = (int)(camera.target.y / tileSize) + (screenHeight / tileSize / 2);
        int startCol = (int)(camera.target.x / tileSize) - (screenWidth/tileSize / 2);
        int endCol = (int)(camera.target.x / tileSize) + (screenWidth / tileSize / 2);

        DrawTiles(startRow, endRow, startCol, endCol, tileSize); // Draw tiles relative to camera
        EndMode2D();

        // Draw the player as a circle for visibility
        DrawCircleV((Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }, 5.0f, RED);
        
        DrawFPS(678, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

