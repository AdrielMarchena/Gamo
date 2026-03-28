#pragma once

#ifdef ENGINE_DEBUG
#define GL_CHECK(x)                                                                                \
    x;                                                                                             \
    {                                                                                              \
        GLenum err = glGetError();                                                                 \
        if (err != GL_NO_ERROR)                                                                    \
        {                                                                                          \
            ENGINE_ASSERT(0 && "OpenGL error");                                                    \
        }                                                                                          \
    }
#else
#define GL_CHECK(x) x
#endif
