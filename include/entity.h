#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>

typedef struct Entity Entity;
struct Entity 
{
    Vector2 position;
    bool isStatic;
    void(*update)(struct Entity* self, float dt);
    void(*draw)(Entity* self);
};

void updateEntity(Entity* entity, float dt);
void drawEntity(Entity* entity);
Entity createEntity(Vector2 position, bool isStatic, void(*update)(Entity* self, float dt), void(*draw)(Entity* self));

#endif