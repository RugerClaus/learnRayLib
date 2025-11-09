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
#define TILES_PER_BIOME 5

typedef struct WorldData 
{
    int seed;
} WorldData;

typedef enum {
    BIOME_OCEAN,
    BIOME_PLAINS,
    BIOME_BEACH,
    BIOME_DESERT,
    BIOME_MOUNTAINS,
    BIOME_COUNT
} Biome;

typedef struct Chunk {
    int tiles[CHUNK_SIZE][CHUNK_SIZE];
    float temperature[CHUNK_SIZE][CHUNK_SIZE];
    Biome biomes[CHUNK_SIZE][CHUNK_SIZE];
    bool isLoaded;
} Chunk;


extern Texture2D tileSheet;
extern const int TILE_PIXELS;
extern Rectangle tileRects[];

// Tile mapping (index into the sprite sheet)
typedef enum {
    TILE_WATER_DEEP,
    TILE_SHORE,
    TILE_SAND,
    TILE_GRASS,
    TILE_HILLS,
    TILE_MOUNTAIN,
    TILE_SNOW,
    TILE_COUNT
} TileType;

typedef enum {
    TEMP_FREEZING,
    TEMP_TEMPERATE,
    TEMP_HOT,
    TEMP_COUNT
} Temperature;


void initTileSheet();
void initRandom();
void initWorld();
int getWorldSeed();
int randRange(int min, int max);
void GenerateChunk(Chunk* chunk, int chunkX, int chunkY);
void UpdateChunks(Player* player);
void DrawChunks(Player* player, Settings* settings);
float getTemperatureAt();
Biome getBiomeAt();

#endif