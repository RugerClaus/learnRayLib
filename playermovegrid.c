#include <raylib.h>

typedef struct Player 
{
    Vector2 position;
    float speed;
} Player;

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

    //setup camera
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f}; //centering the camera i'll play with these values to figure out whats what
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Player player = { .position = {screenWidth / 2.0f, screenHeight / 2.0f}, .speed = 15.0f};

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
        DrawTiles(&rows, &cols, &tileSize); // Draw tiles relative to camera
        EndMode2D();

        // Draw the player as a circle for visibility
        DrawCircleV(player.position, 5.0f, RED);
        
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

