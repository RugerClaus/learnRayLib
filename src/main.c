#include <stdio.h>
#include <raylib.h>
#include <math.h>

#include "../include/state/appstate.h"
#include "../include/player.h"
#include "../include/world.h"
#include "../include/settings.h"

typedef struct Game 
{
    bool isPaused;
    int renderDistance;
    float zoom;
} Game;

typedef struct Debug
{
    bool isEnabled;
    bool showTileOutlines;
    bool showChunkBorders;
} Debug;

Settings settings = 
{
    .renderDistance = 2,
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
    if(settings->renderDistance <= 4)
    {
        settings->renderDistance += 1;
    }
    else
    {
        settings->renderDistance = 1;
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

void handleInput(Player* player, Game* game, Debug* debug, float dt, StateManager* sm)
{

    if (isAppState(sm,MAIN_MENU))
    {
        if (IsKeyPressed(KEY_SPACE)) setAppState(sm,GAME_ACTIVE);
    }

    else if(isAppState(sm, GAME_ACTIVE))
    {
        if(!game->isPaused)
        {
            handlePlayerInput(player, TILE_SIZE);
            if (IsKeyPressed(KEY_R)) 
            {
                Vector2 safePos = findSafeSpawn();
                player->base.position = safePos;
                if (getTileAt((int)safePos.x, (int)safePos.y));
                printf("Player repositioned to safe spawn at (%.2f, %.2f)\n", safePos.x*0.01, safePos.y*0.01);
            }
            
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
            if (IsKeyPressed(KEY_M)) setAppState(sm,MAIN_MENU);
        }
    if (IsKeyPressed(KEY_ESCAPE)) togglePause(game);
    }
}

void UpdateCameraPosition(Camera2D* camera, Player* player) {

    camera->target = (Vector2){ player->base.position.x, player->base.position.y };
    
    camera->offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    player->offset = (Vector2){ camera->target.x - camera->offset.x, camera->target.y - camera->offset.y };

    printf("Camera Target: (%.2f, %.2f)\n", camera->target.x, camera->target.y);
}



void UpdateCameraZoom(Camera2D* camera)
{
    if (IsKeyDown(KEY_KP_ADD)) camera->zoom += 0.05f;
    if (IsKeyDown(KEY_KP_SUBTRACT)) camera->zoom = fmaxf(0.5f, camera->zoom - 0.05f);
    if (IsKeyDown(KEY_KP_0)) camera->zoom = 3.0f;
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

void drawDebug(Player* player, Camera2D* camera)
{
    char positionText[100];
            sprintf(positionText, "Grid Position: (%.2f, %.2f)", player->base.position.x/TILE_SIZE, player->base.position.y/TILE_SIZE);
            DrawText(positionText, 10, 10, 20, BLACK);

            char renderDistanceText[50];
            sprintf(renderDistanceText, "Render Distance: %d", settings.renderDistance);
            DrawText(renderDistanceText, 10, 30, 20, BLACK);

            char worldSeedText[50];
            sprintf(worldSeedText, "World Seed: %d", getWorldSeed());
            DrawText(worldSeedText, 10, 50, 20, BLACK);

            const char* biome = biomeName(getBiomeAt((int)player->base.position.x / TILE_SIZE, (int)player->base.position.y / TILE_SIZE));
            char biomeText[50];
            sprintf(biomeText, "Biome: %s", biome);
            DrawText(biomeText, 10, 70, 20, BLACK);

            char temperature[50];
            sprintf(temperature, "Current Temperature: %.2f", getTemperatureAt((int)player->base.position.x / TILE_SIZE, (int)player->base.position.y / TILE_SIZE));
            DrawText(temperature,10,90,20, BLACK);

            char cameraZoom[50];
            sprintf(cameraZoom, "Camera Zoom: %.2f", camera->zoom);
            DrawText(cameraZoom,10,110,20, BLACK);
            
            DrawFPS(GetScreenWidth()-90, 5);
}

int main(void)
{
    const int defaultScreenWidth = 768;
    const int defaultScreenHeight = 768;
    const int tileSize = TILE_SIZE;

    InitWindow(defaultScreenWidth, defaultScreenHeight, "Distant Realms Raylib");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_F10);
    SetTargetFPS(60);
    initWorld();
    preloadTitleWorld(&settings);
    InitAudioDevice();

    Music music = LoadMusicStream("assets/music/ioa.ogg");
    PlayMusicStream(music);
    SetMusicVolume(music, 1.0f);

    Camera2D camera = { 0 };
    camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
    

    

    
    Vector2 safePos = findSafeSpawn();
    Player player = createPlayer(safePos, 55.0f, GREEN, 12.0f);
    player.base.update = updatePlayer;

    Game game = {
        .isPaused = false,
        .zoom = 3.0f
    };

    Debug debug = {
        .isEnabled = false,
        .showTileOutlines = false,
        .showChunkBorders = false
    };

    StateManager sm = { MAIN_MENU, -1};
    initStateManager(&sm);

    while (!WindowShouldClose())
    {
        
        float dt = GetFrameTime();
        float halfViewW = (GetScreenWidth() * 0.5f) / camera.zoom;
        float halfViewH = (GetScreenHeight() * 0.5f) / camera.zoom;
        handleInput(&player, &game, &debug, dt,&sm);
        UpdateMusicStream(music);
        
        if (isAppState(&sm,MAIN_MENU))
        {
            
            BeginDrawing();
            ClearBackground(BLACK);
            drawTitleScreenMap(&camera, dt);
            UpdateTitleChunks(&camera, &settings); 
            BeginMode2D(camera);
            DrawChunksTitle(&camera, &settings);
            EndMode2D();

            char welcome[50];
            sprintf(welcome, "Welcome to the game, Press Space to continue");

            int textWidth = MeasureText(welcome, 20);
            int textHeight = 20;

            int x = (GetScreenWidth() - textWidth) / 2;
            int y = (GetScreenHeight() - textHeight) / 2;

            DrawText(welcome, x, y, 20, WHITE);
            EndDrawing();
        }
        else if (isAppState(&sm,GAME_ACTIVE))
        {
            if(!game.isPaused)
            {
                UpdateCameraZoom(&camera);
                camera.zoom = game.zoom;
                UpdateCameraPosition(&camera, &player);
                UpdateChunks(&player, &settings);
                updateEntity((Entity*)&player,dt);
            }
            
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
                char pauseText[50];
                sprintf(pauseText, "PAUSED");

                int textWidth = MeasureText(pauseText, 20);
                int textHeight = 20;

                int x = (GetScreenWidth() - textWidth) / 2;
                int y = (GetScreenHeight() - textHeight) / 2;

                DrawText(pauseText, x, y, 40, RED);
            }
            
            
            
            if(debug.isEnabled)
            {
                drawDebug(&player, &camera);
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
        }

        else if (isAppState(&sm,QUIT)) CloseWindow();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
