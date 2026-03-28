#include "engine/renderer/texture.h"
#include "engine/general/engine_alloc.h"

#include <stb_image.h>

#include <glad/glad.h>

#include "gl/gl_check.h"

static unsigned char* load_texture_data(const char* path, int* width, int* height, int* channels)
{
    stbi_set_flip_vertically_on_load(1);
    return stbi_load(path, width, height, channels, 0);
}

static void free_texture_data(unsigned char* data)
{
    stbi_image_free(data);
}

Texture* engine_texture_load(const char* path)
{
    Texture* texture = (Texture*)engine_alloc(sizeof(Texture));

    unsigned char* data =
        load_texture_data(path, &texture->width, &texture->height, &texture->channels);

    if (!data)
    {
        engine_free(texture);
        return NULL;
    }

    GL_CHECK(glGenTextures(1, &texture->id));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture->id));

    /* texture parameters */
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    /* choose format based on channel count */
    GLenum format = GL_RGB;

    if (texture->channels == 1)
    {
        format = GL_RED;
    }
    if (texture->channels == 3)
    {
        format = GL_RGB;
    }
    if (texture->channels == 4)
    {
        format = GL_RGBA;
    }

    /* upload to GPU */
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, texture->width, texture->height, 0,
                          format, GL_UNSIGNED_BYTE, data));

    GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));

    // Free the image data after loading it into the texture
    free_texture_data(data);

    return texture;
}

void engine_texture_bind(Texture* texture, uint32_t slot)
{
    GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture->id));
}

void engine_texture_unbind(uint32_t slot)
{
    GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void engine_texture_destroy(Texture* texture)
{
    if (texture)
    {
        GL_CHECK(glDeleteTextures(1, &texture->id));
        engine_free(texture);
    }
}
