#ifndef WORLD_H
#define WORLD_H
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "settings.h"

#define CHUNK_SIZE 32
#define TILE_SIZE 16
#define WORLD_OFFSET 1000

typedef struct WorldData 
{
    int seed;
} WorldData;

typedef struct Chunk {
    Color tiles[CHUNK_SIZE][CHUNK_SIZE];
    bool isLoaded;
} Chunk;

extern Texture2D tileSheet;
const int TILE_PIXELS;

// Tile mapping (index into the sprite sheet)
enum TileType {
    TILE_WATER,
    TILE_SHORE,
    TILE_SAND,
    TILE_GRASS,
    TILE_HILLS,
    TILE_MOUNTAIN,
    TILE_SNOW,
    TILE_COUNT
};

Rectangle tileRects[TILE_COUNT];

void initTileSheet();
void initRandom();
void initWorld();
int getWorldSeed();
int randRange(int min, int max);
void GenerateChunk(Chunk* chunk, int chunkX, int chunkY);
void UpdateChunks(Player* player);
void DrawChunks(Player* player, Settings* settings);

#endif