#include "../include/player.h"
#include <stdio.h>
#include <raylib.h>

void MovePlayer(Player* player, float dt)
{
    if (player->intent & 1) // Moving Right
    {
        player->state = 1;
        player->base.position.x += player->speed * dt;
    }
    if (player->intent & 2) // Moving Left
    {
        player->state = 2;
        player->base.position.x -= player->speed * dt;
    }
    if (player->intent & 4) // Moving Down
    {
        player->state = 3;
        player->base.position.y += player->speed * dt;
    }
    if (player->intent & 8) // Moving Up
    {
        player->state = 4;
        player->base.position.y -= player->speed * dt;
    }
}

void updatePlayer(Entity* self, float dt)
{
    Player* player = (Player*)self;
    MovePlayer(player, dt);

}

void drawPlayer(Entity* self)
{
    Player* player = (Player*)self;
    
    // Just draw the player based on the player's position
    // Use camera offset during drawing
    Vector2 drawPos = (Vector2){
        player->base.position.x - player->offset.x, 
        player->base.position.y - player->offset.y
    };

    DrawCircleV(drawPos, player->radius, player->color);
}


Player createPlayer(Vector2 position, float speed, Color color, float radius)
{

    Player newPlayer = { 0 };
    newPlayer.base.position = position;
    newPlayer.speed = speed;
    newPlayer.color = color;
    newPlayer.radius = radius;
    newPlayer.intent = 0;
    newPlayer.state = 0;
    return newPlayer;

}