#include "engine/ecs/scene.h"
#include "engine/general/assert.h"
#include "engine/general/engine_alloc.h"

#include "engine/ecs/components/transform.h"
#include "engine/ecs/components/mesh.h"

#include "ecs_world.h"

#include "systems/ecs_render_system.h"

#include <flecs.h>

struct EngineScene
{
    EngineEcsWorld* world;
};

static void register_components(EngineEcsWorld* world)
{
    ecs_world_t* flecs = world->handle;

    ECS_COMPONENT(flecs, TransformComponent);
    ECS_COMPONENT(flecs, MeshComponent);
}

static void register_render_systems(EngineEcsWorld* world)
{
    ecs_world_t* flecs = world->handle;

    ECS_SYSTEM(flecs, engine_ecs_render_system, EcsOnUpdate, TransformComponent, MeshComponent);
}

EngineScene* engine_scene_create(void)
{
    EngineScene* scene = engine_alloc(sizeof(EngineScene));

    if (!scene)
    {
        engine_log_error("Failed to allocate memory for EngineScene.");
        return NULL;
    }

    scene->world = engine_ecs_world_create();

    if (!scene->world)
    {
        engine_free(scene);
        return NULL;
    }

    register_components(scene->world);

    register_render_systems(scene->world);

    return scene;
}

void engine_scene_destroy(EngineScene* scene)
{
    ENGINE_ASSERT(scene != NULL, "Scene cannot be NULL when destroying.");

    engine_ecs_world_destroy(scene->world);
    engine_free(scene);
}

EngineEntity engine_scene_entity_create(EngineScene* scene)
{
    ENGINE_ASSERT(scene != NULL, "Scene cannot be NULL when creating an entity.");

    EngineEntity entity = {0};

    entity.id = ecs_new(scene->world->handle);
    entity.scene = scene;

    return entity;
}

void engine_scene_entity_destroy(EngineEntity entity)
{
    ENGINE_ASSERT(entity.scene != NULL, "Scene cannot be NULL when destroying an entity.");

    ecs_delete(entity.scene->world->handle, entity.id);
}

void* engine_entity_add_component(EngineEntity entity, size_t component_size,
                                  const char* component_name)
{
    ENGINE_ASSERT(entity.scene != NULL, "Scene cannot be NULL when adding a component.");
    ENGINE_ASSERT(component_name != NULL, "Component name cannot be NULL when adding a component.");

    EngineScene* scene = entity.scene;
    EngineEcsWorld* ecs = scene->world;

    ecs_world_t* flecs = ecs->handle;

    ecs_entity_t comp = ecs_lookup(flecs, component_name);

    if (!comp)
    {
        ecs_component_desc_t desc = {0};

        desc.type.size = (ecs_size_t)component_size;

        comp = ecs_component_init(flecs, &desc);
    }

    return ecs_ensure_id(flecs, entity.id, comp, component_size);
}

const void* engine_entity_get_component(EngineEntity entity, const char* component_name)
{
    ENGINE_ASSERT(entity.scene != NULL, "Scene cannot be NULL when getting a component.");
    ENGINE_ASSERT(component_name != NULL,
                  "Component name cannot be NULL when getting a component.");

    EngineScene* scene = entity.scene;
    EngineEcsWorld* ecs = scene->world;

    ecs_world_t* flecs = ecs->handle;

    ecs_entity_t comp = ecs_lookup(flecs, component_name);

    if (!comp)
    {
        return NULL;
    }

    return ecs_get_id(flecs, entity.id, comp);
}

void engine_scene_update(EngineScene* scene, float delta_time)
{
    ENGINE_ASSERT(scene != NULL, "Scene cannot be NULL when updating.");

    EngineEcsWorld* ecs = scene->world;

    ecs_progress(ecs->handle, delta_time);
}