#pragma once

#include <cglm/cglm.h>

#include <glad/glad.h>

typedef struct Shader
{
    GLuint id;

    GLint loc_model;
    GLint loc_view;
    GLint loc_projection;
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

void shader_load_all_from_directory(const char* directory);
