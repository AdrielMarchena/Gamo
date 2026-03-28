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

Framebuffer framebuffer_create(int32_t width, int32_t height);
void framebuffer_resize(Framebuffer* framebuffer, int width, int height);
void framebuffer_destroy(Framebuffer* framebuffer);
void framebuffer_bind(const Framebuffer* framebuffer);
void framebuffer_unbind();
