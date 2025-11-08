#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct Player {
    Vector2 position;
    float speed;
    Color color;
    Color default_color;
    float gridX;
    float gridY;
    float radius;
    int state;
    int intent;
} Player;

void MovePlayer(Player* player, float dt);
void UpdatePlayer(Player* player, float dt);
void DrawPlayer(Player* player, int screenWidth, int screenHeight, int tileSize);

#endif // PLAYER_H
