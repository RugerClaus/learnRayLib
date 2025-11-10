#include "../include/player.h"
#include <stdio.h>
#include <math.h>
#include <raylib.h>

void MovePlayer(Player* player, float dt)
{
    if (player->intent & 1) // Moving Right
    {
        
        player->base.position.x += player->speed * dt;
    }
    if (player->intent & 2) // Moving Left
    {
        
        player->base.position.x -= player->speed * dt;
    }
    if (player->intent & 4) // Moving Down
    {
        
        player->base.position.y += player->speed * dt;
    }
    if (player->intent & 8) // Moving Up
    {
        
        player->base.position.y -= player->speed * dt;
    }
}

void drawUI(Player* player)
{
    const int uiSpacing = 10;
    const int uiTop = 10;
    const int lineHeight = 30;
    const int padding = 20;
    const int margin = 10;
    
    int screenWidth = GetScreenWidth();

    int healthTextWidth = MeasureText(TextFormat("Health: %d", player->health), lineHeight);
    int xpTextWidth = MeasureText(TextFormat("XP: %d", player->exp), lineHeight);
    int levelTextWidth = MeasureText(TextFormat("Level: %d", player->level), lineHeight);

    int maxTextWidth = fmax(healthTextWidth, fmax(xpTextWidth, levelTextWidth));

    int rightX = screenWidth - maxTextWidth - padding - margin;

    int numLines = 3; 
    int totalHeight = numLines * lineHeight + (numLines - 1) * uiSpacing;

    DrawRectangle(rightX - padding, uiTop, maxTextWidth + 2 * padding, totalHeight, Fade(BLACK, 0.6));

    DrawText(TextFormat("Health: %d", player->health), rightX, uiTop + uiSpacing, lineHeight, WHITE);

    DrawText(TextFormat("XP: %d", player->exp), rightX, uiTop + (uiSpacing + lineHeight) * 1, lineHeight, WHITE);

    DrawText(TextFormat("Level: %d", player->level), rightX, uiTop + (uiSpacing + lineHeight) * 1.6, lineHeight, WHITE);
}


void updatePlayer(Entity* self, float dt, int TILE_SIZE)
{
    Player* player = (Player*)self;
    

    int deltaX = 0;
    int deltaY = 0;

    if (IsKeyDown(KEY_W)) deltaY = -1; // Up
    if (IsKeyDown(KEY_S)) deltaY = 1;  // Down
    if (IsKeyDown(KEY_A)) deltaX = -1; // Left
    if (IsKeyDown(KEY_D)) deltaX = 1;  // Right

    int currentTileX = (int)(player->base.position.x / TILE_SIZE);
    int currentTileY = (int)(player->base.position.y / TILE_SIZE);

    int intendedTileX = currentTileX + deltaX;
    int intendedTileY = currentTileY + deltaY;

    TileType nextTile = getTileAt(intendedTileX, intendedTileY);

    if(nextTile == TILE_WATER_DEEP) {
        // Can't move here
        printf("Blocked! Tile is water.\n");
    } else {
        // Move the player
        player->base.position.x += deltaX * TILE_SIZE;
        player->base.position.y += deltaY * TILE_SIZE;
    }

    MovePlayer(player, dt);

}

void drawPlayer(Entity* self)
{
    Player* player = (Player*)self;
    
    Vector2 drawPos = (Vector2){
        player->base.position.x - player->offset.x, 
        player->base.position.y - player->offset.y
    };

    DrawCircleV(drawPos, player->radius, player->color);

    drawUI(player);
}

void handlePlayerInput(Player* player, int TILE_SIZE)
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

    if (IsKeyPressed(KEY_LEFT_SHIFT))
    {
        player->speed *= 2.0f;
    }
    if (IsKeyReleased(KEY_LEFT_SHIFT))
    {
        player->speed *= 0.5f;
    }

    if (IsKeyPressed(KEY_LEFT_CONTROL))
    {
        player->speed *= 0.5f;
    }
    if (IsKeyReleased(KEY_LEFT_CONTROL))
    {
        player->speed *= 2.0f;
    }
}


Player createPlayer(Vector2 position, float speed, Color color, float radius)
{

    Player newPlayer = { 0 };
    newPlayer.base.position = position;
    newPlayer.speed = speed;
    newPlayer.color = color;
    newPlayer.radius = radius;
    newPlayer.intent = 0;
    newPlayer.level = 0;
    newPlayer.exp = 0;
    newPlayer.health = 10;
    newPlayer.biomeName[0] = '\0';
    newPlayer.tileName[0] = '\0';
    return newPlayer;

}