#pragma once

#if (LRU == 1)
#pragma message "Using LRU cache"
#else
#pragma message "Using non-LRU cache"
#endif

#include "cache_notempl_wrapper.h"
#include "cache_notempl_manager.h"
#include "cache_notempl_cache.h"
