#pragma once

#include <string_view>

namespace kae {

enum class LogLevel {
    debug,
    info,
    warning,
    error,
    fatal,
    none,

#ifdef DEBUG
    default_level = debug,
#else
    default_level = info,
#endif
};


LogLevel global_log_level();                   ///< Thread-safe
LogLevel tag_log_level(std::string_view tag);  ///< Thread-safe

void set_global_log_level(LogLevel level);                     ///< Thread-safe
void set_tag_log_level(std::string_view tag, LogLevel level);  ///< Thread-safe
void reset_tag_log_level(std::string_view tag);                ///< Thread-safe

}  // namespace kae
