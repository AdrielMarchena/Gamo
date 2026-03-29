#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t fb_object;
    uint32_t color_attachment;
    uint32_t rb_object;
    int32_t width;
    int32_t height;
} Framebuffer;

Framebuffer* engine_framebuffer_create(int32_t width, int32_t height);
void engine_framebuffer_resize(Framebuffer* framebuffer, int width, int height);
void engine_framebuffer_destroy(Framebuffer* framebuffer);
void engine_framebuffer_bind(const Framebuffer* framebuffer);
void engine_framebuffer_unbind();
