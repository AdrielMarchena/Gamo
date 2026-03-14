#pragma once

#include "engine/platform/window.h"
#include "engine/renderer/texture.h"

#include <stdbool.h>

#include <cglm/cglm.h>

#include "mesh.h"

#include "engine/ecs/components/camera.h"

// Renderer API

bool engine_renderer_init(EngineWindow* window);
void engine_renderer_shutdown(void);

void engine_renderer_begin(void);
void engine_renderer_end(void);

// Draw functions

void engine_renderer_draw_quad(float xpos, float ypos, float width, float height);

void engine_renderer_draw_mesh(const mat4* model, const Mesh* mesh);
void engine_renderer_draw_mesh_with_texture(const mat4* model, const Mesh* mesh, Texture* texture,
                                            const vec4* color);

void engine_renderer_set_camera(const EngineCamera* camera);
EngineCamera* engine_renderer_get_camera(void);

// Viewport

void engine_renderer_resize(int width, int height);
void engine_renderer_set_clear_color(float red, float green, float blue, float alpha);
void engine_renderer_clear_color(void);

// UI Integration - Handled internally by the engine
// UI is automatically initialized and rendered through the backend system

void engine_renderer_handle_ui_input(void);
void engine_renderer_draw_ui(void);
