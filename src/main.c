#include <stdio.h>
#include <raylib.h>
#include <math.h>

#include "../include/player.h"
#include "../include/world.h"
#include "../include/settings.h"

typedef struct Game 
{
    bool isPaused;
    int renderDistance;
} Game;

typedef struct Debug
{
    bool isEnabled;
    bool showTileOutlines;
    bool showChunkBorders;
} Debug;

Settings settings = 
{
    .renderDistance = 5,
};

void toggleTileOutlines(Debug* debug)
{
    printf("tile outlines toggled\n");
    if(debug->showTileOutlines)
    {
        debug->showTileOutlines = false;
    }
    else
    {
        debug->showTileOutlines = true;
    }
}

void toggleRenderDistance(Settings* settings)
{
    if(settings->renderDistance < 10)
    {
        settings->renderDistance += 1;
    }
    else
    {
        settings->renderDistance = 5;
    }
}

void DrawTileOutlines(int startRow, int endRow, int startCol, int endCol, int tileSize)
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

void toggleChunkBorders(Debug* debug)
{
    if(debug->showChunkBorders)
    {
        debug->showChunkBorders = false;
    }
    else
    {
        debug->showChunkBorders = true;
    }
}

void DrawChunkBorders(int startRow, int endRow, int startCol, int endCol, int tileSize)
{
    int chunkPixelSize = CHUNK_SIZE * tileSize;

    // Convert from tile coordinates to chunk coordinates
    int startChunkX = startCol / CHUNK_SIZE;
    int endChunkX   = endCol   / CHUNK_SIZE;
    int startChunkY = startRow / CHUNK_SIZE;
    int endChunkY   = endRow   / CHUNK_SIZE;

    for (int chunkY = startChunkY; chunkY <= endChunkY; chunkY++)
    {
        for (int chunkX = startChunkX; chunkX <= endChunkX; chunkX++)
        {
            int x = chunkX * chunkPixelSize;
            int y = chunkY * chunkPixelSize;

            DrawRectangleLines(x, y, chunkPixelSize, chunkPixelSize, RED);
        }
    }
}


void togglePause(Game* game)
{
    if(game->isPaused)
    {
        game->isPaused = false;
    }
    else
    {
        game->isPaused = true;
    }
}

void handleInput(Player* player, Game* game, Debug* debug, float dt)
{
    if(!game->isPaused)
    {
        player->intent = 0;
        if (IsKeyDown(KEY_D)) player->intent |= 1;
        if (IsKeyDown(KEY_A)) player->intent |= 2;
        if (IsKeyDown(KEY_S)) player->intent |= 4;
        if (IsKeyDown(KEY_W)) player->intent |= 8;
        if (IsKeyPressed(KEY_BACKSLASH))
        {
            player->base.position.x = 100*TILE_SIZE;
            player->base.position.y = 100*TILE_SIZE;
        }
        if (IsKeyReleased(KEY_D) && player->intent == 1) player->intent = 0;
        if (IsKeyReleased(KEY_A) && player->intent == 2) player->intent = 0;
        if (IsKeyReleased(KEY_S) && player->intent == 4) player->intent = 0;
        if (IsKeyReleased(KEY_W) && player->intent == 8) player->intent = 0;
        if (IsKeyPressed(KEY_P)) togglePause(game);
        if (IsKeyPressed(KEY_F3)) debug->isEnabled = !debug->isEnabled;
        if (debug->isEnabled)
        {
            if (IsKeyPressed(KEY_T)) toggleTileOutlines(debug);
            if (IsKeyPressed(KEY_C)) toggleChunkBorders(debug);
        }
        if (IsKeyPressed(KEY_F)) toggleRenderDistance(&settings);
    }
    else
    {
        if (IsKeyPressed(KEY_P)) togglePause(game);
    }
}

void UpdateCameraPosition(Camera2D* camera, Player* player) {
    // Follow the player
    camera->target = (Vector2){ player->base.position.x, player->base.position.y };
    
    // Optionally, keep the camera centered on the player
    camera->offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    player->offset = (Vector2){ camera->target.x - camera->offset.x, camera->target.y - camera->offset.y };

    printf("Camera Target: (%.2f, %.2f)\n", camera->target.x, camera->target.y);
}



void UpdateCameraZoom(Camera2D* camera)
{
    if (IsKeyDown(KEY_KP_ADD)) camera->zoom += 0.05f;
    if (IsKeyDown(KEY_KP_SUBTRACT)) camera->zoom = fmaxf(0.5f, camera->zoom - 0.05f);
    if (IsKeyDown(KEY_KP_0)) camera->zoom = 1.0f;
}

const char* biomeName(Biome biome) {
    switch(biome)
    {
        case BIOME_OCEAN: return "Ocean";
        case BIOME_BEACH: return "Beach";
        case BIOME_PLAINS: return "Plains";
        case BIOME_DESERT: return "Desert"; 
        case BIOME_MOUNTAINS: return "Mountains";
        default: return "Unknown";
    }
}

int main(void)
{
    // Initialize window
    const int screenWidth = 768;
    const int screenHeight = 768;
    const int tileSize = TILE_SIZE;

    InitWindow(screenWidth, screenHeight, "Distant Realms Raylib");
    SetTargetFPS(60);
    initWorld();

    // Setup camera
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    

    

    

    Player player = createPlayer((Vector2){100.0f * tileSize, 100.0f * tileSize}, 200.0f, GREEN, 12.0f);
    player.base.update = updatePlayer;

    Game game = {
        .isPaused = false,
    };

    Debug debug = {
        .isEnabled = false,
        .showTileOutlines = false,
        .showChunkBorders = false
    };

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        float halfViewW = (screenWidth * 0.5f) / camera.zoom;
        float halfViewH = (screenHeight * 0.5f) / camera.zoom;
        if(!game.isPaused)
        {
            
            UpdateCameraPosition(&camera, &player);
            UpdateCameraZoom(&camera);
            UpdateChunks(&player);
            updateEntity((Entity*)&player,dt);
        }
        
        handleInput(&player, &game, &debug, dt);

        BeginDrawing();
        ClearBackground(BLUE);
        
        BeginMode2D(camera);
        DrawChunks(&player, &settings);

        int startRow = (int)floor(player.base.position.y / tileSize - halfViewH / tileSize) - 1;
        int endRow   = (int)ceil (player.base.position.y / tileSize + halfViewH / tileSize) + 1;
        int startCol = (int)floor(player.base.position.x / tileSize - halfViewW / tileSize) - 1;
        int endCol   = (int)ceil (player.base.position.x / tileSize + halfViewW / tileSize) + 1;
        if(debug.showTileOutlines)
        {
            DrawTileOutlines(startRow, endRow, startCol, endCol, tileSize);
        }
        if(debug.showChunkBorders)
        {
            DrawChunkBorders(startRow, endRow, startCol, endCol, tileSize);
        }
        
        EndMode2D();

        drawPlayer((Entity*)&player);
        
        if (game.isPaused) {
            DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 40) / 2, screenHeight / 2 - 20, 40, RED);
        }
        
        
        
        if(debug.isEnabled)
        {
            char positionText[100];
            sprintf(positionText, "Grid Position: (%.2f, %.2f)", player.base.position.x/TILE_SIZE, player.base.position.y/TILE_SIZE);
            DrawText(positionText, 10, 10, 20, BLACK);

            char renderDistanceText[50];
            sprintf(renderDistanceText, "Render Distance: %d", settings.renderDistance);
            DrawText(renderDistanceText, 10, 30, 20, BLACK);

            char worldSeedText[50];
            sprintf(worldSeedText, "World Seed: %d", getWorldSeed());
            DrawText(worldSeedText, 10, 50, 20, BLACK);

            const char* biome = biomeName(getBiomeAt((int)player.base.position.x / TILE_SIZE, (int)player.base.position.y / TILE_SIZE));
            char biomeText[50];
            sprintf(biomeText, "Biome: %s", biome);
            DrawText(biomeText, 10, 70, 20, BLACK);

            char temperature[50];
            sprintf(temperature, "Current Temperature: %.2f", getTemperatureAt((int)player.base.position.x / TILE_SIZE, (int)player.base.position.y / TILE_SIZE));
            DrawText(temperature,10,90,20, BLACK);
            
            DrawFPS(678, 5);
        }
        else
        {
            if(debug.showChunkBorders)
            {
                toggleChunkBorders(&debug);
            }
            else if (debug.showTileOutlines)
            {
                toggleTileOutlines(&debug);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
