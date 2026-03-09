#pragma once

#include <cglm/cglm.h>
#include "engine/renderer/texture.h"

typedef struct Shader
{
    uint32_t id;

    int32_t loc_model;
    int32_t loc_view;
    int32_t loc_projection;
    int32_t loc_texture;
    int32_t loc_color;
} Shader;

Shader* engine_gl_shader_create(const char* vertex_source, const char* fragment_source);
void engine_gl_shader_destroy(Shader* shader);
void engine_gl_shader_set_uniform_mat4(Shader* shader, const char* name, const mat4* matrix);
void engine_gl_shader_bind(const Shader* shader);
void engine_gl_shader_unbind(void);
Shader* engine_gl_shader_create_from_files(const char* vertex_path, const char* fragment_path);

void engine_gl_shader_set_model(Shader* shader, const mat4* model);
void engine_gl_shader_set_view(Shader* shader, const mat4* view);
void engine_gl_shader_set_projection(Shader* shader, const mat4* projection);

void engine_gl_shader_set_texture(Shader* shader, const Texture* texture);
void engine_gl_shader_set_color(Shader* shader, const vec4* color);

void shader_load_all_from_directory(const char* directory);
