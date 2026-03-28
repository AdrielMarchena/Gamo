#pragma once

#ifdef ENGINE_DEBUG
#include <assert.h>
#include "engine/general/logger.h"
#define ENGINE_ASSERT(cond, msg)                                                                   \
    do                                                                                             \
    {                                                                                              \
        if (!(cond))                                                                               \
        {                                                                                          \
            engine_log_error("ASSERT FAILED: %s\nFile: %s\nLine: %d\n", msg, __FILE__, __LINE__);  \
            assert(cond);                                                                          \
        }                                                                                          \
    } while (0)
#else
#define ENGINE_ASSERT(cond, msg)                                                                   \
    do                                                                                             \
    {                                                                                              \
        (void)(cond);                                                                              \
    } while (0)
#endif
