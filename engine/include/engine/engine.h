#pragma once

#include "engine/platform/window.h"
#include "engine/events/events.h"
#include "engine/renderer/renderer.h"
#include "engine/ecs/scene.h"

typedef struct Engine
{
    EngineWindow* window;
    EngineRenderer* renderer;
    EventQueue* event_queue;
    EngineScene* scene;
} Engine;

/********************
 * Engine Functions *

This file declares the main functions for creating and destroying the Engine instance.

It's main purpose is to provide a bunch of contexts such as the window, renderer, event queue, etc.
to be used across the engine.

So, each system will have its own context struct, and the Engine struct will hold pointers to those
contexts.

 ********************/

Engine* engine_create();
void engine_destroy(Engine* engine);

// Window context
EngineWindow* engine_get_window(Engine* engine);

// Renderer context
EngineRenderer* engine_get_renderer(Engine* engine);

// Event context
EventQueue* engine_get_event_queue(Engine* engine);

// Scene context
EngineScene* engine_get_scene(Engine* engine);
