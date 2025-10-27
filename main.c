#include <raylib.h>

void handlePlayerInput(int *rectx, int *recty, float speed, float dt)
{
    if(IsKeyDown(KEY_W))
    {
        *recty -= speed * dt;
    }
    else if(IsKeyDown(KEY_S))
    {
        *recty += speed * dt;
    }
    else if(IsKeyDown(KEY_A))
    {
        *rectx -= speed * dt;
    }
    else if(IsKeyDown(KEY_D))
    {
        *rectx += speed * dt;
    }
}

int main(void)
{
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 450;
    SetTargetFPS(60);

    //rectangle stuff
    int rectx = 200;
    int recty = 200;

    float speed = 200.0f;
    


    InitWindow(screenWidth, screenHeight, "Hello Raylib!");

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
        handlePlayerInput(&rectx,&recty,speed,dt);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(rectx, recty, 10, 10, GREEN);
        DrawFPS(725,5);
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}

