#include "shader.h"

#include "cglm/common.h"
#include "engine/general/engine_alloc.h"
#include "engine/general/read_file.h"

#include "engine/general/logger.h"

#include "shader_uniforms.h"
#include "shader_registry.h"

#include <dirent.h>
#include <string.h>
#include <stdio.h>

#include "glad/glad.h"

Shader* engine_gl_shader_create(const char* vertex_source, const char* fragment_source)
{
    Shader* shader = engine_alloc(sizeof(Shader));

    GLuint vprog = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vprog, 1, &vertex_source, NULL);
    glCompileShader(vprog);

    GLuint fprog = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fprog, 1, &fragment_source, NULL);
    glCompileShader(fprog);

    shader->id = glCreateProgram();
    glAttachShader(shader->id, vprog);
    glAttachShader(shader->id, fprog);
    glLinkProgram(shader->id);

    glDeleteShader(vprog);
    glDeleteShader(fprog);

    shader->loc_model = glGetUniformLocation(shader->id, U_MODEL);
    shader->loc_view = glGetUniformLocation(shader->id, U_VIEW);
    shader->loc_projection = glGetUniformLocation(shader->id, U_PROJECTION);

    shader->loc_texture = glGetUniformLocation(shader->id, U_TEXTURE);
    shader->loc_color = glGetUniformLocation(shader->id, U_COLOR);

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
    assert(shader != NULL);
    glUseProgram(shader->id);
}

void engine_gl_shader_unbind(void)
{
    glUseProgram(0);
}

Shader* engine_gl_shader_create_from_files(const char* vertex_path, const char* fragment_path)
{
    char* vertex_source = engine_read_file(vertex_path);
    char* fragment_source = engine_read_file(fragment_path);

    if (!vertex_source)
    {
        engine_log_error("Failed to read vertex shader file: %s\n", vertex_path);
        return NULL;
    }

    if (!fragment_source)
    {
        engine_log_error("Failed to read fragment shader file: %s\n", fragment_path);
        return NULL;
    }

    Shader* shader = engine_gl_shader_create(vertex_source, fragment_source);

    if (!shader)
    {
        engine_log_error("Failed to create shader from files: %s, %s\n", vertex_path,
                         fragment_path);
        return NULL;
    }

    engine_free(vertex_source);
    engine_free(fragment_source);

    return shader;
}

void engine_gl_shader_set_model(Shader* shader, const mat4* model)
{
    if (shader->loc_model == -1)
    {
        engine_log_warning("Model uniform not found in shader (ID: %u)\n", shader->id);
        return;
    }
    glUniformMatrix4fv(shader->loc_model, 1, GL_FALSE, (const GLfloat*)model);
}

void engine_gl_shader_set_view(Shader* shader, const mat4* view)
{
    if (shader->loc_view == -1)
    {
        engine_log_warning("View uniform not found in shader (ID: %u)\n", shader->id);
        return;
    }
    glUniformMatrix4fv(shader->loc_view, 1, GL_FALSE, (const GLfloat*)view);
}

void engine_gl_shader_set_projection(Shader* shader, const mat4* projection)
{
    if (shader->loc_projection == -1)
    {
        engine_log_warning("Projection uniform not found in shader (ID: %u)\n", shader->id);
        return;
    }
    glUniformMatrix4fv(shader->loc_projection, 1, GL_FALSE, (const GLfloat*)projection);
}

void engine_gl_shader_set_texture(Shader* shader, const Texture* texture)
{
    if (shader->loc_texture == -1)
    {
        engine_log_warning("Texture uniform not found in shader (ID: %u)\n", shader->id);
        return;
    }
    glUniform1i(shader->loc_texture, 0); // Texture unit 0
}

void engine_gl_shader_set_color(Shader* shader, const vec4* color)
{
    if (shader->loc_color == -1)
    {
        engine_log_warning("Color uniform not found in shader (ID: %u)\n", shader->id);
        return;
    }
    glUniform4fv(shader->loc_color, 1, (const GLfloat*)color);
}

void shader_load_all_from_directory(const char* directory)
{
    DIR* dir = opendir(directory);
    if (!dir)
    {
        engine_log_error("Failed to open shader directory: %s\n", directory);
        return;
    }

    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (!engine_file_has_extension(entry->d_name, ".vert"))
        {
            continue;
        }

        char base[256];
        strncpy(base, entry->d_name, sizeof(base));
        base[strlen(base) - 5] = '\0'; // remove ".vert"

        char vert_path[512];
        char frag_path[512];

        snprintf(vert_path, sizeof(vert_path), "%s/%s.vert", directory, base);

        snprintf(frag_path, sizeof(frag_path), "%s/%s.frag", directory, base);

        // check fragment exists
        FILE* file = fopen(frag_path, "r");
        if (!file)
        {
            continue;
        }

        fclose(file);

        Shader* shader = engine_gl_shader_create_from_files(vert_path, frag_path);

        // store shader in your renderer registry
        engine_shader_registry_add(base, shader);
    }

    closedir(dir);
}