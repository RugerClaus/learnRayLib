#include "../include/world.h"
#include <stdio.h>
#include <math.h>
#include <raylib.h>

Chunk** chunks;

WorldData worldData;


Texture2D tileSheet;
const int TILE_PIXELS = 16;
Rectangle tileRects[TILE_COUNT];

Rectangle biomeTileRects[BIOME_COUNT][TILES_PER_BIOME];

float biomeMinHeight[BIOME_COUNT] = {
    [BIOME_OCEAN]     = 0.0f,
    [BIOME_BEACH]     = 0.3f,
    [BIOME_PLAINS]    = 0.35f,
    [BIOME_DESERT]    = 0.35f,
    [BIOME_MOUNTAINS]= 0.75f
};

float biomeMaxHeight[BIOME_COUNT] = {
    [BIOME_OCEAN]     = 0.3f,
    [BIOME_BEACH]     = 0.35f,
    [BIOME_PLAINS]    = 0.75f,
    [BIOME_DESERT]    = 0.75f,
    [BIOME_MOUNTAINS]= 1.0f
};

void initTileSheet() {
    tileSheet = LoadTexture("assets/tilesheet.png");
    if (tileSheet.id == 0) {
        printf("Failed to load tilesheet!\n");
        return;
    }

    // Build the rects for each biome + variation
    for (int biome = 0; biome < BIOME_COUNT; biome++) {
        for (int i = 0; i < TILES_PER_BIOME; i++) {
            biomeTileRects[biome][i] = (Rectangle){
                i * TILE_PIXELS,        // column
                biome * TILE_PIXELS,    // row
                TILE_PIXELS,
                TILE_PIXELS
            };
        }
    }
}
int randRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

void InitRandom() {
    srand(time(NULL)); // seed PRNG with current time
}

int setWorldSeed(int seed) {
    worldData.seed = seed;
}

int getWorldSeed() {
    return worldData.seed;
}

void initWorld() {
    InitRandom();
    int seed = randRange(100000, 999999);
    setWorldSeed(seed);
    initTileSheet();
    chunks = malloc(sizeof(Chunk*) * 500);
    for (int i = 0; i < 500; i++) {
        chunks[i] = malloc(sizeof(Chunk) * 500);
    }
}

static float Noise2D_Seeded(int x, int y)
{
    int n = x + y * 57 + worldData.seed * 131;
    n = (n << 13) ^ n;
    int nn = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return 1.0f - ((float)nn / 1073741824.0f);
}

static float Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

static float SmoothNoise(float x, float y)
{
    int intX = (int)x;
    int intY = (int)y;
    float fracX = x - intX;
    float fracY = y - intY;

    float v1 = Noise2D_Seeded(intX, intY);
    float v2 = Noise2D_Seeded(intX + 1, intY);
    float v3 = Noise2D_Seeded(intX, intY + 1);
    float v4 = Noise2D_Seeded(intX + 1, intY + 1);

    float i1 = Lerp(v1, v2, fracX);
    float i2 = Lerp(v3, v4, fracX);
    return Lerp(i1, i2, fracY);
}

static float FractalNoise(float x, float y, int octaves)
{
    float total = 0.0f;
    float frequency = 0.9f;
    float amplitude = 1.0f;

    for (int i = 0; i < octaves; ++i)
    {
        total += SmoothNoise(x * frequency, y * frequency) * amplitude;
        frequency *= 2.0f;
        amplitude *= 0.5f;
    }

    return total;
}

void GenerateChunk(Chunk* chunk, int chunkX, int chunkY)
{
    for (int row = 0; row < CHUNK_SIZE; row++)
    {
        for (int col = 0; col < CHUNK_SIZE; col++)
        {
            int worldX = chunkX * CHUNK_SIZE + col;
            int worldY = chunkY * CHUNK_SIZE + row;

            float nx = worldX / 100.0f;
            float ny = worldY / 100.0f;

            float height = (FractalNoise(nx, ny, 4) + 1.0f) / 2.0f;
            float tempNorm = (FractalNoise(nx + WORLD_OFFSET, ny + WORLD_OFFSET, 3) + 1.0f) / 2.0f;
            float tempC = -10.0f + tempNorm * 50.0f;
            tempC -= height * 20.0f;
            chunk->temperature[row][col] = tempC;

            Biome biome;
            if (height < 0.3f) biome = BIOME_OCEAN;
            else if (height < 0.35f) biome = BIOME_BEACH;
            else if (height > 0.75f) biome = BIOME_MOUNTAINS;
            else
            {
                if (tempC < 0.3f) biome = BIOME_MOUNTAINS;
                else if (tempC < 0.6f) biome = BIOME_PLAINS;
                else biome = BIOME_DESERT;
            }
            chunk->biomes[row][col] = biome;

            float biomeHeight = (height - biomeMinHeight[biome]) /
                                (biomeMaxHeight[biome] - biomeMinHeight[biome]);
            if (biomeHeight < 0) biomeHeight = 0;
            if (biomeHeight > 1) biomeHeight = 1;

            int variant = (int)(biomeHeight * TILES_PER_BIOME);
            if (variant >= TILES_PER_BIOME) variant = TILES_PER_BIOME - 1;

            chunk->tiles[row][col] = variant;
        }
    }

    chunk->isLoaded = true;
}



void UpdateChunks(Player* player, Settings* settings) {
    int playerChunkX = (int)(player->base.position.x / (CHUNK_SIZE * TILE_SIZE));
    int playerChunkY = (int)(player->base.position.y / (CHUNK_SIZE * TILE_SIZE));

    for (int dx = -settings->renderDistance; dx <= settings->renderDistance; dx++) {
        for (int dy = -settings->renderDistance; dy <= settings->renderDistance; dy++) {
            int chunkX = playerChunkX + dx;
            int chunkY = playerChunkY + dy;

            if (chunkX >= 0 && chunkY >= 0 && chunkX < 100 && chunkY < 100) {
                Chunk* chunk = &chunks[chunkX][chunkY];
                if (!chunk->isLoaded) {
                    GenerateChunk(chunk, chunkX, chunkY);
                }
            }
        }
    }


    for (int chunkX = 0; chunkX < 100; chunkX++) {
        for (int chunkY = 0; chunkY < 100; chunkY++) {
            Chunk* chunk = &chunks[chunkX][chunkY];

            int distX = abs(chunkX - playerChunkX);
            int distY = abs(chunkY - playerChunkY);
            if (distX > settings->renderDistance || distY > settings->renderDistance) {
                if (chunk->isLoaded) {
                    
                    chunk->isLoaded = false;
                    printf("Unloading chunk: (%d, %d)\n", chunkX, chunkY);
                }
            }
        }
    }

    
    printf("Current chunk: (%d, %d)\n", playerChunkX, playerChunkY);
}


void DrawChunks(Player* player, Settings* settings) {
    int playerChunkX = (int)(player->base.position.x / (CHUNK_SIZE * TILE_SIZE));
    int playerChunkY = (int)(player->base.position.y / (CHUNK_SIZE * TILE_SIZE));

    for (int dx = -settings->renderDistance; dx <= settings->renderDistance; dx++) {
        for (int dy = -settings->renderDistance; dy <= settings->renderDistance; dy++) {
            int chunkX = playerChunkX + dx;
            int chunkY = playerChunkY + dy;

            if (chunkX >= 0 && chunkY >= 0 && chunkX < 100 && chunkY < 100) {
                Chunk* chunk = &chunks[chunkX][chunkY];
                if (chunk->isLoaded) {
                    for (int row = 0; row < CHUNK_SIZE; row++) {
                        for (int col = 0; col < CHUNK_SIZE; col++) {
                            int variant = chunk->tiles[row][col];
                            Biome biome = chunk->biomes[row][col];

                            DrawTextureRec(
                                tileSheet,
                                biomeTileRects[biome][variant],
                                (Vector2){ 
                                    (chunkX * CHUNK_SIZE + col) * TILE_SIZE, 
                                    (chunkY * CHUNK_SIZE + row) * TILE_SIZE
                                },
                                WHITE
                            );
                        }
                    }
                }
            }
        }
    }
}

void drawTitleScreenMap(Camera2D* camera, float dt)
{
    static float time = 0.0f;
    time += dt;

    Vector2 center = { 250 * CHUNK_SIZE * TILE_SIZE / 2.0f, 
                    250 * CHUNK_SIZE * TILE_SIZE / 2.0f };

    float radius = 200.0f;
    float speed = 0.5f;

    camera->target.x = center.x + cosf(time * speed) * radius;
    camera->target.y = center.y + sinf(time * speed) * radius;

    
    camera->zoom = 0.9f + 0.1f * sinf(time * 0.3f);

    camera->offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
}

void DrawChunksTitle(Camera2D* camera, Settings* settings)
{
    int camChunkX = (int)(camera->target.x / (CHUNK_SIZE * TILE_SIZE));
    int camChunkY = (int)(camera->target.y / (CHUNK_SIZE * TILE_SIZE));

    for (int dx = -settings->renderDistance; dx <= settings->renderDistance; dx++) {
        for (int dy = -settings->renderDistance; dy <= settings->renderDistance; dy++) {
            int chunkX = camChunkX + dx;
            int chunkY = camChunkY + dy;

            if (chunkX >= 0 && chunkY >= 0 && chunkX < 500 && chunkY < 500) {
                Chunk* chunk = &chunks[chunkX][chunkY];
                if (chunk->isLoaded) {
                    for (int row = 0; row < CHUNK_SIZE; row++) {
                        for (int col = 0; col < CHUNK_SIZE; col++) {
                            int variant = chunk->tiles[row][col];
                            Biome biome = chunk->biomes[row][col];
                            DrawTextureRec(
                                tileSheet,
                                biomeTileRects[biome][variant],
                                (Vector2){
                                    (chunkX * CHUNK_SIZE + col) * TILE_SIZE,
                                    (chunkY * CHUNK_SIZE + row) * TILE_SIZE
                                },
                                WHITE
                            );
                        }
                    }
                }
            }
        }
    }
}

void UpdateTitleChunks(Camera2D* camera, Settings* settings)
{
    int camWorldX = (int)(camera->target.x);
    int camWorldY = (int)(camera->target.y);

    int centerChunkX = camWorldX / (CHUNK_SIZE * TILE_SIZE);
    int centerChunkY = camWorldY / (CHUNK_SIZE * TILE_SIZE);

    for (int dx = -settings->renderDistance; dx <= settings->renderDistance; dx++) {
        for (int dy = -settings->renderDistance; dy <= settings->renderDistance; dy++) {
            int chunkX = centerChunkX + dx;
            int chunkY = centerChunkY + dy;

            if (chunkX >= 0 && chunkY >= 0 && chunkX < 500 && chunkY < 500) {
                Chunk* chunk = &chunks[chunkX][chunkY];
                if (!chunk->isLoaded) {
                    GenerateChunk(chunk, chunkX, chunkY);
                }
            }
        }
    }
}

void preloadTitleWorld(Settings* settings)
{
    int centerX = 250;
    int centerY = 250;
    int preloadRadius = 3;

    for (int dx = -preloadRadius; dx <= preloadRadius; dx++) {
        for (int dy = -preloadRadius; dy <= preloadRadius; dy++) {
            int chunkX = centerX + dx;
            int chunkY = centerY + dy;
            Chunk* chunk = &chunks[chunkX][chunkY];
            GenerateChunk(chunk, chunkX, chunkY);
        }
    }
}

Chunk* getChunk(int chunkX, int chunkY) {
    if (chunkX < 0 || chunkY < 0 || chunkX >= 500 || chunkY >= 500) {
        return NULL;
    }

    Chunk* chunk = &chunks[chunkX][chunkY];

    if (!chunk->isLoaded) {
        GenerateChunk(chunk, chunkX, chunkY);
    }

    return chunk;
}

float getTemperatureAt(int worldX, int worldY) {
    int chunkX = worldX / CHUNK_SIZE;
    int chunkY = worldY / CHUNK_SIZE;
    int localX = worldX % CHUNK_SIZE;
    int localY = worldY % CHUNK_SIZE;

    Chunk* chunk = getChunk(chunkX, chunkY);
    if (!chunk) return -1.0f;

    return chunk->temperature[localY][localX];
}

Biome getBiomeAt(int worldX, int worldY) {
    int chunkX = worldX / CHUNK_SIZE;
    int chunkY = worldY / CHUNK_SIZE;
    int localX = worldX % CHUNK_SIZE;
    int localY = worldY % CHUNK_SIZE;

    Chunk* chunk = getChunk(chunkX, chunkY);
    if (!chunk) return BIOME_OCEAN;

    return chunk->biomes[localY][localX];
}
TileType getTileAt(int worldX, int worldY) {
    int chunkX = worldX / CHUNK_SIZE;
    int chunkY = worldY / CHUNK_SIZE;
    int localX = worldX % CHUNK_SIZE;
    int localY = worldY % CHUNK_SIZE;

    Chunk* chunk = getChunk(chunkX, chunkY);
    if (!chunk) return TILE_WATER_DEEP;

    return chunk->tiles[localY][localX];
}

Vector2 findSafeSpawn() {
    Vector2 spawn;
    TileType tile;

    do {
        spawn.x = rand() % (64 * TILE_SIZE);
        spawn.y = rand() % (64 * TILE_SIZE);

        int tileX = (int)(spawn.x / TILE_SIZE);
        int tileY = (int)(spawn.y / TILE_SIZE);
        tile = getTileAt((int)spawn.x, (int)spawn.y);
    } while(tile == TILE_WATER_DEEP || tile == TILE_SHORE);

    return spawn;
}
