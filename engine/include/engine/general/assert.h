#pragma once

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
