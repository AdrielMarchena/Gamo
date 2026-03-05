#include "ecs_world.h"

#include "engine/general/engine_alloc.h"

#include <flecs.h>

EngineEcsWorld* engine_ecs_world_create()
{
    EngineEcsWorld* world = (EngineEcsWorld*)engine_alloc(sizeof(EngineEcsWorld));

    if (!world)
    {
        return NULL;
    }

    world->handle = ecs_init();

    if (!world->handle)
    {
        engine_free(world);
        return NULL;
    }

    return world;
}

void engine_ecs_world_destroy(EngineEcsWorld* world)
{
    if (!world)
    {
        return;
    }

    ecs_fini(world->handle);
    engine_free(world);
}
