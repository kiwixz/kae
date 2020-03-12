#pragma once

#include <exception>

#include "utils/logger.h"


#define DETAIL_PP_STRINGIFY(a) #a
#define STRINGIFY(a) DETAIL_PP_STRINGIFY(a)

#define SOURCE_LOCATION __FILE__ ":" STRINGIFY(__LINE__)


#ifdef DEBUG
#    define ASSERT(condition)                                                                                                                \
        do {                                                                                                                                 \
            if (!(condition)) {                                                                                                              \
                ::utils::Logger{"assert"}(::utils::LogLevel::fatal, "condition is false: " STRINGIFY(condition) "\nfrom: " SOURCE_LOCATION); \
                ::std::terminate();                                                                                                          \
            }                                                                                                                                \
        } while (false)
#else
#    define ASSERT(condition) ((void)0)
#endif
