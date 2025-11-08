#include "../include/player.h"
#include <stdio.h>
#include <raylib.h>

void MovePlayer(Player* player, float dt)
{
    if (player->intent & 1) // Moving Right
    {
        player->state = 1;
        player->position.x += player->speed * dt;
    }
    if (player->intent & 2) // Moving Left
    {
        player->state = 2;
        player->position.x -= player->speed * dt;
    }
    if (player->intent & 4) // Moving Down
    {
        player->state = 3;
        player->position.y += player->speed * dt;
    }
    if (player->intent & 8) // Moving Up
    {
        player->state = 4;
        player->position.y -= player->speed * dt;
    }
}

void UpdatePlayer(Player* player, float dt)
{
    
    player->gridX = player->position.x / 32;
    player->gridY = player->position.y / 32;
    MovePlayer(player, dt);
}

void DrawPlayer(Player* player, int screenWidth, int screenHeight, int tileSize)
{
    
    
    player->gridX = (player->position.x / tileSize);
    player->gridY = (player->position.y / tileSize);

    DrawCircleV((Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }, player->radius, player->color);
    
}