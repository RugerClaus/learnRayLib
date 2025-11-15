#ifndef WORLD_H
#define WORLD_H
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include "settings.h"
#include "entity.h"

#define CHUNK_SIZE 16
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
    TILE_WATER_OCEAN_5,
    TILE_WATER_OCEAN_4,
    TILE_WATER_OCEAN_3,
    TILE_WATER_OCEAN_2,
    TILE_WATER_OCEAN_1,
    TILE_PLAINS_5,
    TILE_PLAINS_4,
    TILE_PLAINS_3,
    TILE_PLAINS_2,
    TILE_PLAINS_1,
    TILE_WATER_SHALLOW,
    TILE_BEACH_4,
    TILE_BEACH_3,
    TILE_BEACH_2,
    TILE_BEACH_1,
    TILE_DESERT_5,
    TILE_DESERT_4,
    TILE_DESERT_3,
    TILE_DESERT_2,
    TILE_DESERT_1,
    TILE_MOUNTAIN_5,
    TILE_MOUNTAIN_4,
    TILE_MOUNTAIN_3,
    TILE_MOUNTAIN_2,
    TILE_MOUNTAIN_1,
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
int setWorldSeed(int seed);
int randRange(int min, int max);
TileType generateBiomeTiles(Biome biome,float height);
void GenerateChunk(Chunk* chunk, int chunkX, int chunkY);
void UpdateChunks(Entity* entity, Settings* settings);
void DrawChunks(Entity* entity, Settings* settings);
void drawTitleScreenMap(Camera2D* camera, float dt);
void DrawChunksTitle(Camera2D* camera, Settings* settings);
void UpdateTitleChunks(Camera2D* camera, Settings* settings);
void preloadTitleWorld(Settings* settings);
Chunk* getChunk(int chunkX, int chunkY);
float getTemperatureAt();
Biome getBiomeAt();
TileType getTileAt(int worldX, int worldY);
Vector2 findSafeSpawn();

#endif