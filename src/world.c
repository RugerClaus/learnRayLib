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

// perlin noise experiment functions
// A simple pseudo-random hash function
static float Noise2D(int x, int y) {
    int n = x + y * 57 + worldData.seed * 131;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 81722 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

// Smooth interpolation between two values
static float Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Generate smooth noise by interpolating between Noise2D grid points
static float SmoothNoise(float x, float y) {
    int intX = (int)x;
    int intY = (int)y;
    float fracX = x - intX;
    float fracY = y - intY;

    float v1 = Noise2D(intX, intY);
    float v2 = Noise2D(intX + 1, intY);
    float v3 = Noise2D(intX, intY + 1);
    float v4 = Noise2D(intX + 1, intY + 1);

    float i1 = Lerp(v1, v2, fracX);
    float i2 = Lerp(v3, v4, fracX);
    return Lerp(i1, i2, fracY);
}

void GenerateChunk(Chunk* chunk, int chunkX, int chunkY) {
    for (int row = 0; row < CHUNK_SIZE; row++) {
        for (int col = 0; col < CHUNK_SIZE; col++) {
            int worldX = chunkX * CHUNK_SIZE + col;
            int worldY = chunkY * CHUNK_SIZE + row;

            float nx = worldX / 100.0f;
            float ny = worldY / 100.0f;

            float height = (SmoothNoise(nx, ny) + 1.0f) / 2.0f;
            float tempNorm = (SmoothNoise(nx + WORLD_OFFSET, ny + WORLD_OFFSET) + 1.0f) / 2.0f;

            // Map normalized temp to Celsius
            float minTemp = -10.0f;
            float maxTemp = 40.0f;
            float tempC = minTemp + tempNorm * (maxTemp - minTemp);

            // Reduce temp with elevation
            tempC -= height * 20.0f;

            chunk->temperature[row][col] = tempC;


            // --- Determine biome using height and temperature ---
            Biome biome;
            if (height < 0.3f) {
                biome = BIOME_OCEAN;
            } 
            else if (height < 0.35f) {
                biome = BIOME_BEACH;
            } 
            else if (height > 0.75f) {
                biome = BIOME_MOUNTAINS;
            } 
            else {
                if (tempC < 0.3f) biome = BIOME_MOUNTAINS;
                else if (tempC < 0.6f) biome = BIOME_PLAINS;
                else biome = BIOME_DESERT;
            }

            // --- Store biome ---
            chunk->biomes[row][col] = biome;

            // --- Determine tile variant ---
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


void UpdateChunks(Player* player) {
    int playerChunkX = (int)(player->position.x / (CHUNK_SIZE * TILE_SIZE));
    int playerChunkY = (int)(player->position.y / (CHUNK_SIZE * TILE_SIZE));

    // Check surrounding chunks (3x3 area)
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int chunkX = playerChunkX + dx;
            int chunkY = playerChunkY + dy;

            // Check chunk bounds
            if (chunkX >= 0 && chunkY >= 0 && chunkX < 100 && chunkY < 100) {
                Chunk* chunk = &chunks[chunkX][chunkY];
                if (!chunk->isLoaded) {
                    GenerateChunk(chunk,chunkX,chunkY); // Generate chunk if not done
                }
            }
        }
    }
    printf("Current chunk: (%d, %d)\n", playerChunkX, playerChunkY);
}

void DrawChunks(Player* player, Settings* settings) {
    int playerChunkX = (int)(player->position.x / (CHUNK_SIZE * TILE_SIZE));
    int playerChunkY = (int)(player->position.y / (CHUNK_SIZE * TILE_SIZE));

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
                            Biome biome = chunk->biomes[row][col];  // Use stored biome

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

float getTemperatureAt(int worldX, int worldY) {
    int chunkX = worldX / CHUNK_SIZE;
    int chunkY = worldY / CHUNK_SIZE;
    int localX = worldX % CHUNK_SIZE;
    int localY = worldY % CHUNK_SIZE;

    // bounds check
    if (chunkX < 0 || chunkY < 0 || chunkX >= 500 || chunkY >= 500) return -1;

    Chunk* chunk = &chunks[chunkX][chunkY];

    // make sure the chunk is generated
    if (!chunk->isLoaded) {
        GenerateChunk(chunk, chunkX, chunkY);
    }

    return chunk->temperature[localY][localX];
}

Biome getBiomeAt(int worldX, int worldY) {
    int chunkX = worldX / CHUNK_SIZE;
    int chunkY = worldY / CHUNK_SIZE;
    int localX = worldX % CHUNK_SIZE;
    int localY = worldY % CHUNK_SIZE;

    // bounds check
    if (chunkX < 0 || chunkY < 0 || chunkX >= 500 || chunkY >= 500) return BIOME_OCEAN;

    Chunk* chunk = &chunks[chunkX][chunkY];

    // make sure the chunk is generated
    if (!chunk->isLoaded) {
        GenerateChunk(chunk, chunkX, chunkY);
    }

    return chunk->biomes[localY][localX];
}
