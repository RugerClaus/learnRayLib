#include <stdio.h>
#include <raylib.h>
#include <math.h>

typedef struct Player 
{
    Vector2 position;
    float speed;
    Color color;
    Color default_color;
    float gridX;
    float gridY;
} Player;

int blueOneX = 50;
int blueOneY = 1;

Rectangle rRect;

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

void UpdatePlayer(Player* player, float dt)
{
    if (IsKeyDown(KEY_RIGHT)) player->position.x += player->speed * dt;
    if (IsKeyDown(KEY_LEFT)) player->position.x -= player->speed * dt;
    if (IsKeyDown(KEY_DOWN)) player->position.y += player->speed * dt;
    if (IsKeyDown(KEY_UP)) player->position.y -= player->speed * dt;
    if (IsKeyPressed(KEY_BACKSLASH))
    {
        player->position.x = 0;
        player->position.y = 0;
    }
    player->gridX = player->position.x / 32;
    player->gridY = player->position.y / 32;
}

void UpdateCameraPosition(Camera2D* camera, Vector2 playerPosition)
{
    camera->target = playerPosition;
}

void UpdateCameraZoom(Camera2D* camera)
{
    if (IsKeyDown(KEY_KP_ADD)) camera->zoom += 0.05f;
    if (IsKeyDown(KEY_KP_SUBTRACT)) camera->zoom = fmaxf(0.5f, camera->zoom - 0.05f);
    if (IsKeyDown(KEY_KP_0)) camera->zoom = 1.0f;
}

void DrawPlayer(Player* player, int screenWidth, int screenHeight, int tileSize)
{
    
    
    player->gridX = (player->position.x / tileSize);
    player->gridY = (player->position.y / tileSize);

    DrawCircleV((Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }, 10.0f, player->color);

    // Display grid coordinates
    char positionText[100];
    sprintf(positionText, "Grid Position: (%.1f, %.1f)", player->gridX, player->gridY);
    DrawText(positionText, 10, 10, 20, BLACK);
}

int main(void)
{
    // Initialize window
    const int screenWidth = 768;
    const int screenHeight = 768;
    const int tileSize = 32;

    int tileX = 10;
    int tileY = 1;
    rRect = (Rectangle){tileX * tileSize, tileY * tileSize,tileSize,tileSize};

    InitWindow(screenWidth, screenHeight, "Hello Raylib!");
    SetTargetFPS(60);

    // Setup camera
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    

    int startXTile = 0;
    int startYTile = 0;
    

    
    Player player = { 
        .position = {(float)(startXTile * tileSize), (float)(startYTile * tileSize)},
        .speed = 150.0f,
        .color = BLUE,
        .default_color = BLUE
    };

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        float halfViewW = (screenWidth * 0.5f) / camera.zoom;
        float halfViewH = (screenHeight * 0.5f) / camera.zoom;
        UpdateCameraZoom(&camera);
        UpdatePlayer(&player, dt);
        UpdateCameraPosition(&camera, player.position);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode2D(camera);

        int startRow = (int)floor(player.position.y / tileSize - halfViewH / tileSize) - 1;
        int endRow   = (int)ceil (player.position.y / tileSize + halfViewH / tileSize) + 1;
        int startCol = (int)floor(player.position.x / tileSize - halfViewW / tileSize) - 1;
        int endCol   = (int)ceil (player.position.x / tileSize + halfViewW / tileSize) + 1;

        DrawTiles(startRow, endRow, startCol, endCol, tileSize); 
        DrawRectangleRec(rRect,GREEN);

        if(CheckCollisionCircleRec(player.position,10.0f,rRect))
        {
            player.color = PURPLE;
        }
        else player.color = player.default_color;
        EndMode2D();

        DrawPlayer(&player, screenWidth, screenHeight, tileSize);
        
        
        char positionText[100];
        sprintf(positionText, "Zoom Offset: (%.1f,%.1f)", halfViewW,halfViewH);
        DrawText(positionText, 10, 60, 20, BLACK);

        DrawFPS(678, 5);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
