#pragma once

#include <stdint.h>

typedef struct Texture
{
    uint32_t id;
    int32_t width;
    int32_t height;
    int32_t channels;
} Texture;

Texture* engine_texture_load(const char* path);
void engine_texture_bind(Texture*, uint32_t slot);
void engine_texture_unbind(uint32_t slot);
void engine_texture_destroy(Texture* texture);
