#include "engine/renderer/framebuffer.h"
#include "engine/general/engine_alloc.h"
#include "engine/general/assert.h"
#include "engine/general/logger.h"
#include "gl_check.h"

#include "glad/glad.h"

static Framebuffer* framebuffer_internal_allocation()
{
    Framebuffer* framebuffer = (Framebuffer*)engine_alloc(sizeof(Framebuffer));
    if (!framebuffer)
    {
        engine_log_error("Failed to allocate memory for framebuffer");
        return NULL;
    }

    framebuffer->width = 0;
    framebuffer->height = 0;
    framebuffer->fb_object = 0;
    framebuffer->color_attachment = 0;
    framebuffer->rb_object = 0;

    return framebuffer;
}

static void framebuffer_internal_framebuffer_texture_attachment(Framebuffer* framebuffer)
{
    GL_CHECK(glGenTextures(1, &framebuffer->color_attachment));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, framebuffer->color_attachment));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer->width, framebuffer->height, 0,
                          GL_RGB, GL_UNSIGNED_BYTE, NULL));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                    framebuffer->color_attachment, 0));
}

static void framebuffer_internal_renderbuffer_attachment(Framebuffer* framebuffer)
{
    GL_CHECK(glGenRenderbuffers(1, &framebuffer->rb_object));
    GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rb_object));
    GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer->width,
                                   framebuffer->height));
    GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                       framebuffer->rb_object));
}

static void framebuffer_internal_gl_creation(Framebuffer* framebuffer, int32_t width,
                                             int32_t height)
{
    ENGINE_ASSERT(framebuffer->fb_object == 0, "Framebuffer object is already created, if you want "
                                               "to recreate it, destroy the existing one first");

    framebuffer->width = width;
    framebuffer->height = height;

    GL_CHECK(glGenFramebuffers(1, &framebuffer->fb_object));

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fb_object));

    framebuffer_internal_framebuffer_texture_attachment(framebuffer);
    framebuffer_internal_renderbuffer_attachment(framebuffer);

    GL_CHECK(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    ENGINE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

static void invalidate_framebuffer(Framebuffer* framebuffer)
{
    if (framebuffer)
    {
        GL_CHECK(glDeleteFramebuffers(1, &framebuffer->fb_object));
        GL_CHECK(glDeleteTextures(1, &framebuffer->color_attachment));
        GL_CHECK(glDeleteRenderbuffers(1, &framebuffer->rb_object));

        framebuffer->width = 0;
        framebuffer->height = 0;
        framebuffer->fb_object = 0;
        framebuffer->color_attachment = 0;
        framebuffer->rb_object = 0;
    }
}

Framebuffer* engine_framebuffer_create(int32_t width, int32_t height)
{
    ENGINE_ASSERT(width >= 0 && height >= 0, "Framebuffer width and height must be greater than 0");

    if (width <= 0 || height <= 0) // Runtime check
    {
        engine_log_error("Failed to create framebuffer: invalid dimensions (%d, %d)", width,
                         height);
        return NULL;
    }

    Framebuffer* framebuffer = framebuffer_internal_allocation();

    if (!framebuffer)
    {
        return NULL;
    }

    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->fb_object = 0;
    framebuffer->color_attachment = 0;
    framebuffer->rb_object = 0;

    framebuffer_internal_gl_creation(framebuffer, width, height);

    return framebuffer;
}

void engine_framebuffer_resize(Framebuffer* framebuffer, int width, int height)
{
    ENGINE_ASSERT(framebuffer != NULL, "Framebuffer pointer is NULL");
    ENGINE_ASSERT(width >= 0 && height >= 0, "Framebuffer width and height must be greater than 0");

    if (!framebuffer)
    {
        engine_log_error("Failed to resize framebuffer: framebuffer pointer is NULL");
        return;
    }

    if (width <= 0 || height <= 0) // Runtime check
    {
        engine_log_error("Failed to resize framebuffer: invalid dimensions (%d, %d)", width,
                         height);
        return;
    }

    invalidate_framebuffer(framebuffer);

    if (!framebuffer)
    {
        return;
    }

    framebuffer->width = width;
    framebuffer->height = height;

    framebuffer_internal_gl_creation(framebuffer, width, height);
}

void engine_framebuffer_destroy(Framebuffer* framebuffer)
{
    if (framebuffer)
    {
        invalidate_framebuffer(framebuffer);
        engine_free(framebuffer);
        framebuffer = NULL;
    }
}

void engine_framebuffer_bind(const Framebuffer* framebuffer)
{
    ENGINE_ASSERT(framebuffer != NULL, "Framebuffer pointer is NULL");
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fb_object));
}

void engine_framebuffer_unbind()
{
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
