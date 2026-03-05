#include "shader_registry.h"
#include "engine/general/logger.h"
#include "engine/general/engine_alloc.h"
#include "shader.h"

#include <string.h>
#include <stdio.h>

#define INITIAL_SHADER_REGISTRY_CAPACITY 10;

typedef struct ShaderEntry
{
    char* name;
    Shader* shader;
} ShaderEntry;

static ShaderEntry* shader_registry = NULL;
static size_t shader_registry_count = 0;
static size_t shader_registry_capacity = 0;

void engine_shader_registry_init()
{
    shader_registry_capacity = INITIAL_SHADER_REGISTRY_CAPACITY;
    shader_registry_count = 0;
    shader_registry = (ShaderEntry*)engine_alloc(shader_registry_capacity * sizeof(ShaderEntry));
}

void engine_shader_registry_destroy()
{
    for (size_t i = 0; i < shader_registry_count; i++)
    {
        engine_free(shader_registry[i].name);
        engine_gl_shader_destroy(shader_registry[i].shader);
    }
    engine_free(shader_registry);
    shader_registry = NULL;
    shader_registry_count = 0;
    shader_registry_capacity = 0;
}

void engine_shader_registry_add(const char* name, Shader* shader)
{
    if (shader_registry_count >= shader_registry_capacity)
    {
        shader_registry_capacity *= 2;
        shader_registry =
            engine_realloc(shader_registry, shader_registry_capacity * sizeof(ShaderEntry));
    }

    shader_registry[shader_registry_count].name = strdup(name);
    shader_registry[shader_registry_count].shader = shader;
    shader_registry_count++;
}

Shader* engine_shader_registry_get(const char* name)
{
    for (size_t i = 0; i < shader_registry_count; i++)
    {
        if (strcmp(shader_registry[i].name, name) == 0)
        {
            return shader_registry[i].shader;
        }
    }
    engine_log_warn("Shader not found in registry: %s\n", name);
    return NULL;
}