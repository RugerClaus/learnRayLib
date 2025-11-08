#ifndef WORLD_H
#define WORLD_H
#include <raylib.h>
#include "player.h"
#include "settings.h"

#define CHUNK_SIZE 16
#define TILE_SIZE 32
#define WORLD_OFFSET 1000

typedef struct Chunk {
    Color tiles[CHUNK_SIZE][CHUNK_SIZE];
    bool isLoaded;
} Chunk;

void GenerateChunk(Chunk* chunk);
void UpdateChunks(Player* player);
void DrawChunks(Player* player, Settings* settings);

#endif