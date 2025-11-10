#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "world.h"
#include <raylib.h>
#include <stdio.h>


typedef struct Player {
    Entity base;
    float speed;
    Color color;
    Color default_color;
    float radius;
    int intent;
    Vector2 offset;
    int health;
    int level;
    int exp;
    float temperature;
    char biomeName[32];
    char tileName[32];
} Player;

void DrawPlayerInfo(Player* player);
void updatePlayer(Entity* self, float dt);
void drawPlayer(Entity* self);
void handlePlayerInput(Player* player);
void MovePlayer(Player* player, float dt);
void drawUI(Player* player);
Player createPlayer(Vector2 position, float speed, Color color, float radius);

#endif
