#pragma once

#include <flecs.h>

typedef struct EngineEcsWorld
{
    ecs_world_t* handle;
} EngineEcsWorld;

EngineEcsWorld* engine_ecs_world_create(void);
void engine_ecs_world_destroy(EngineEcsWorld* world);
