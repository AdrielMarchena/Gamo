#include "engine/events/events.h"

typedef struct UpdateData
{
    float delta_time;
    uint64_t frame_count;
    float frame_rate;

} UpdateData;

typedef struct EngineLayer
{
    void (*on_create)(struct EngineLayer* layer);
    void (*on_event)(struct EngineLayer* layer, Event* event);
    void (*on_update)(struct EngineLayer* layer, UpdateData update_data);
    void (*on_render)(struct EngineLayer* layer);
    void (*on_destroy)(struct EngineLayer* layer);

    void* userdata; // optional, per-layer state

    uint64_t id; // Unique identifier for the layer, can be used for sorting or other purposes, do
                 // not change it manually, it is set by the engine when the layer is pushed
} EngineLayer;
