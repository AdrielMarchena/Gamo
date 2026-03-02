#include "shader.h"

#include "engine/general/engine_alloc.h"

#include "glad/glad.h"

Shader* engine_gl_shader_create(const char* vertex_source, const char* fragment_source)
{
    Shader* shader = engine_alloc(sizeof(Shader));

    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vertex_source, NULL);
    glCompileShader(v);

    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fragment_source, NULL);
    glCompileShader(f);

    shader->id = glCreateProgram();
    glAttachShader(shader->id, v);
    glAttachShader(shader->id, f);
    glLinkProgram(shader->id);

    glDeleteShader(v);
    glDeleteShader(f);

    return shader;
}

void engine_gl_shader_destroy(Shader* shader)
{
    glDeleteProgram(shader->id);
    engine_free(shader);
}

void engine_gl_shader_set_uniform_mat4(Shader* shader, const char* name, const mat4* matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, (const GLfloat*)matrix);
}

void engine_gl_shader_bind(const Shader* shader)
{
    glUseProgram(shader->id);
}

void engine_gl_shader_unbind(void)
{
    glUseProgram(0);
}
