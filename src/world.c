#include "../include/world.h"
#include <stdio.h>
#include <math.h>
#include <raylib.h>

Chunk chunks[500][500];

void GenerateChunk(Chunk* chunk) {
    for (int row = 0; row < CHUNK_SIZE; row++) {
        for (int col = 0; col < CHUNK_SIZE; col++) {
            if (col < 0 || row < 0) {
                chunk->tiles[row][col] = BLUE; // Mark as water tiles
            } else {
                chunk->tiles[row][col] = BROWN; // Land tiles
            }
        }
    }
    chunk->isLoaded = true; // Mark this chunk as generated
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
                    GenerateChunk(chunk); // Generate chunk if not done
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
                            DrawRectangle(
                                (chunkX * CHUNK_SIZE * TILE_SIZE) + (col * TILE_SIZE),
                                (chunkY * CHUNK_SIZE * TILE_SIZE) + (row * TILE_SIZE),
                                TILE_SIZE, TILE_SIZE, chunk->tiles[row][col]);
                        }
                    }
                }
            }
        }
    }
}