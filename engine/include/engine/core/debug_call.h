#pragma once

#ifdef ENGINE_DEBUG
#define ENGINE_DEBUG_ONLY(x) x
#else
#define ENGINE_DEBUG_ONLY(x)
#endif