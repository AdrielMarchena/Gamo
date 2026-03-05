#pragma once

#include "shader.h"

void engine_shader_registry_init(void);
void engine_shader_registry_destroy(void);

void engine_shader_registry_add(const char* name, Shader* shader);
Shader* engine_shader_registry_get(const char* name);
