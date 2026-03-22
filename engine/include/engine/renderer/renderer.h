#pragma once

#include "engine/platform/window.h"
#include "engine/renderer/texture.h"
#include "engine/ui/ui.h"

#include <stdbool.h>

#include <cglm/cglm.h>

#include "mesh.h"

#include "engine/ecs/components/camera.h"

typedef struct EngineRenderer EngineRenderer;

// Renderer API

EngineRenderer* engine_renderer_init(EngineWindow* window, EngineUI* ui_context);
void engine_renderer_shutdown(EngineRenderer* renderer);

void engine_renderer_begin(EngineRenderer* renderer);
void engine_renderer_end(EngineRenderer* renderer);

// Draw functions

void engine_renderer_draw_quad(EngineRenderer* renderer, float xpos, float ypos, float width,
                               float height);

void engine_renderer_draw_mesh(EngineRenderer* renderer, const mat4* model, const Mesh* mesh);
void engine_renderer_draw_mesh_with_texture(EngineRenderer* renderer, const mat4* model,
                                            const Mesh* mesh, Texture* texture, const vec4* color);

void engine_renderer_set_camera(EngineRenderer* renderer, const EngineCamera* camera);
EngineCamera* engine_renderer_get_camera(EngineRenderer* renderer);

// Viewport

void engine_renderer_resize(EngineRenderer* renderer, int width, int height);
void engine_renderer_set_clear_color(EngineRenderer* renderer, float red, float green, float blue,
                                     float alpha);
void engine_renderer_clear_color(EngineRenderer* renderer);

// UI Integration - Handled internally by the engine
// UI is automatically initialized and rendered through the backend system

void engine_renderer_handle_ui_input(EngineRenderer* renderer);
void engine_renderer_draw_ui(EngineRenderer* renderer);
