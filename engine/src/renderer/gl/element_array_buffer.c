#include "element_array_buffer.h"
#include "gl_check.h"

#include "engine/general/engine_alloc.h"

#include "glad/glad.h"

ElementArrayBuffer* engine_gl_element_array_buffer_create(const void* data, size_t size)
{
    ElementArrayBuffer* element_array_buffer = engine_alloc(sizeof(ElementArrayBuffer));
    element_array_buffer->type = GL_ELEMENT_ARRAY_BUFFER;

    GL_CHECK(glGenBuffers(1, &element_array_buffer->id));
    GL_CHECK(glBindBuffer(element_array_buffer->type, element_array_buffer->id));
    GL_CHECK(glBufferData(element_array_buffer->type, (GLsizeiptr)size, data, GL_STATIC_DRAW));
    element_array_buffer->size = (GLsizeiptr)size;
    return element_array_buffer;
}

void engine_gl_element_array_buffer_destroy(ElementArrayBuffer* element_array_buffer)
{
    GL_CHECK(glDeleteBuffers(1, &element_array_buffer->id));
    engine_free(element_array_buffer);
}

void engine_gl_element_array_buffer_bind(const ElementArrayBuffer* element_array_buffer)
{
    GL_CHECK(glBindBuffer(element_array_buffer->type, element_array_buffer->id));
}

void engine_gl_element_array_buffer_unbind(void)
{
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void engine_gl_element_array_buffer_update(ElementArrayBuffer* element_array_buffer,
                                           const void* data, size_t size)
{
    GL_CHECK(glBindBuffer(element_array_buffer->type, element_array_buffer->id));
    GL_CHECK(glBufferSubData(element_array_buffer->type, 0, (GLsizeiptr)size, data));
    element_array_buffer->size = (GLsizeiptr)size;
}

unsigned int engine_gl_element_array_buffer_get_size(const ElementArrayBuffer* element_array_buffer)
{
    return (unsigned int)element_array_buffer->size;
}