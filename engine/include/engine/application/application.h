#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "engine/engine.h"
#include "layer.h"

#define ENGINE_MAX_LAYERS 16

typedef struct
{
    EngineLayer layers[ENGINE_MAX_LAYERS];
    EngineLayer ui_layers[ENGINE_MAX_LAYERS];
    int count;
    int ui_count;
} LayerStack;

typedef struct EngineApp
{
    bool (*init)(void);
    void (*shutdown)(void);

    Engine* engine;
    LayerStack layer_stack;
} EngineApp;

int engine_run(const EngineApp* app);
EngineApp* engine_create_app(bool (*init)(void), void (*shutdown)(void));
void engine_app_destroy(EngineApp* app);
EngineApp* engine_get_current_app();
Engine* engine_get_current_engine();

/*
 * Pushes a layer onto the application's layer stack. The layer will be updated and rendered in the
 * order it was added. UI layers are rendered on top of regular layers. The layer's on_event,
 * on_update, on_render, and on_destroy callbacks will be called at the appropriate times during the
 * application's lifecycle.
 * It also ads a unique identifier to the layer, which can be used for sorting or other purposes. Do
 * not change the id manually, it is set by the engine when the layer is pushed.
 */
void engine_push_layer(EngineApp* app, EngineLayer* layer);
/**
 * Pushes a UI layer onto the application's layer stack. UI layers are rendered on top of regular
 * layers. The layer's on_event, on_update, on_render, and on_destroy callbacks will be called at
 * the appropriate times during the application's lifecycle.
 * It also ads a unique identifier to the layer, which can be used for sorting or other purposes. Do
 * not change the id manually, it is set by the engine when the layer is pushed.
 */
void engine_push_ui_layer(EngineApp* app, EngineLayer* layer);

void engine_pop_layer(EngineApp* app, uint64_t layer_id);
void engine_pop_ui_layer(EngineApp* app, uint64_t layer_id);
