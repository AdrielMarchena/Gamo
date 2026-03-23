#pragma once

#include <stdint.h>
#include <stddef.h>

#include "engine/events/events.h"

#define engine_entity_add(entity, T) (T*)engine_entity_add_component(entity, sizeof(T), #T)

#define engine_entity_get(entity, T) ((const T*)engine_entity_get_component(entity, #T))

typedef struct EngineScene EngineScene;

typedef struct
{
    uint64_t id;
    EngineScene* scene;
} EngineEntity;

EngineScene* engine_scene_create(void);
void engine_scene_destroy(EngineScene* scene);

EngineEntity engine_scene_entity_create(EngineScene* scene);
void engine_scene_entity_destroy(EngineEntity entity);

void* engine_entity_add_component(EngineEntity entity, size_t component_size,
                                  const char* component_name);

const void* engine_entity_get_component(EngineEntity entity, const char* component_name);

void engine_scene_update(EngineScene* scene, float delta_time);

void engine_scene_handle_event(EngineScene* scene, const Event* event);
