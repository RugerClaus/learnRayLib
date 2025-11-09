#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include <raylib.h>

typedef struct Player {
    Entity base;
    float speed;
    Color color;
    Color default_color;
    float radius;
    int state;
    int intent;
    Vector2 offset;
} Player;

void updatePlayer(Entity* self, float dt);
void drawPlayer(Entity* self);
Player createPlayer(Vector2 position, float speed, Color color, float radius);

#endif // PLAYER_H
