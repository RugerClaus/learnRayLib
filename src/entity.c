#include "../include/entity.h"

void updateEntity(Entity* entity, float dt)
{
    if (entity && entity->update)
    {
        entity->update(entity,dt);
    }
}

void drawEntity(Entity* entity)
{
    if(entity && entity->draw)
    {
        entity->draw(entity);
    }
}

Entity createEntity(Vector2 position, bool isStatic, void(*update)(Entity* self, float dt), void(*draw)(Entity* self))
{
    Entity newEntity = { 0 };
    newEntity.position = position;
    newEntity.isStatic = isStatic;
    newEntity.update = update;
    newEntity.draw = draw;
    return newEntity;
};