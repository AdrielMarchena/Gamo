#pragma once

#include <cglm/cglm.h>

#include <glad/glad.h>

typedef struct Shader
{
    GLuint id;

} Shader;

Shader* engine_gl_shader_create(const char* vertex_source, const char* fragment_source);
void engine_gl_shader_destroy(Shader* shader);
void engine_gl_shader_set_uniform_mat4(Shader* shader, const char* name, const mat4* matrix);
void engine_gl_shader_bind(const Shader* shader);
void engine_gl_shader_unbind(void);
