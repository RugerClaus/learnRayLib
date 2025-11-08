#include "../include/world.h"
#include <stdio.h>
#include <math.h>
#include <raylib.h>

Chunk chunks[500][500];

WorldData worldData;


Texture2D tileSheet;
const int TILE_PIXELS = 16;
Rectangle tileRects[TILE_COUNT];

// Example of loading it in your init function
void initTileSheet() {
    tileSheet = LoadTexture("assets/tilesheet.png"); // load your sprite sheet
    if (tileSheet.id == 0) printf("Failed to load tilesheet!\n");

    // Set source rectangles for each tile in the sheet
    for (int i = 0; i < TILE_COUNT; i++) {
        tileRects[i] = (Rectangle){ i * TILE_PIXELS, 0, TILE_PIXELS, TILE_PIXELS };
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
}

// perlin noise experiment functions
// A simple pseudo-random hash function
static float Noise2D(int x, int y) {
    int n = x + y * 57 + worldData.seed * 131;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
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

            // Compute global world coordinates (so chunks line up!)
            int worldX = chunkX * CHUNK_SIZE + col;
            int worldY = chunkY * CHUNK_SIZE + row;

            // Scale down to make smooth continents instead of noisy mess
            float nx = worldX / 100.0f;
            float ny = worldY / 100.0f;

            float height = (SmoothNoise(nx, ny) + 1) / 2.0f; // Normalize to [0,1]

            int tileType;
            if (height < 0.3f) tileType = TILE_WATER_DEEP;       // Deep ocean
            else if (height < 0.35f) tileType = TILE_SHORE;    // Shoreline
            else if (height < 0.45f) tileType = TILE_SAND;     // Beach
            else if (height < 0.6f) tileType = TILE_GRASS;       // Grassland
            else if (height < 0.75f) tileType = TILE_HILLS;      // Hills
            else if (height < 0.8f) tileType = TILE_MOUNTAIN;  // Mountains
            else tileType = TILE_SNOW;      // Snowy peaks

            chunk->tiles[row][col] = tileType;
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

void DrawChunks(Player* player, Settings* settings) 
{
    int playerChunkX = (int)(player->position.x / (CHUNK_SIZE * TILE_SIZE));
    int playerChunkY = (int)(player->position.y / (CHUNK_SIZE * TILE_SIZE));

    // Only draw loaded chunks in a 3x3 area around the player
    for (int dx = -settings->renderDistance; dx <= settings->renderDistance; dx++) {
        for (int dy = -settings->renderDistance; dy <= settings->renderDistance; dy++) {
            int chunkX = playerChunkX + dx;
            int chunkY = playerChunkY + dy;

            if (chunkX >= 0 && chunkY >= 0 && chunkX < 100 && chunkY < 100) {
                Chunk* chunk = &chunks[chunkX][chunkY];
                if (chunk->isLoaded) {
                    for (int row = 0; row < CHUNK_SIZE; row++) {
                        for (int col = 0; col < CHUNK_SIZE; col++) {
                            TileType tileType = chunk->tiles[row][col];

                            DrawTextureRec(
                                tileSheet,
                                tileRects[tileType],  // rectangle for this tile type
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