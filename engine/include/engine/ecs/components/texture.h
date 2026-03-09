#pragma once

#include "engine/renderer/texture.h"
#include <cglm/cglm.h>

typedef struct TextureComponent
{
    Texture* texture;
    vec4 color;
} TextureComponent;
