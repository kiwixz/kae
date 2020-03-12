#pragma once

#include <string_view>

#include <fmt/format.h>

#include "utils/logging.h"
#include "utils/span.h"

namespace utils {

struct Logger {
    explicit Logger(std::string tag);

    bool operator()(LogLevel level, std::string_view message) const;

    template <typename... Args>
    bool operator()(LogLevel level, std::string_view format, Args&&... args) const;

    bool hexdump(LogLevel level, Span<const std::byte> data, std::string_view title) const;

    template <typename... Args>
    bool hexdump(LogLevel level, Span<const std::byte> data, std::string_view format, Args&&... args) const;

private:
    std::string tag_;

    void sink(LogLevel level, std::string_view message) const;
    void sink_hexdump(LogLevel level, Span<const std::byte> data, std::string_view title) const;
};


template <typename... Args>
bool Logger::operator()(LogLevel level, std::string_view format, Args&&... args) const
{
    if (level < tag_log_level(tag_))
        return false;

    sink(level, fmt::format(format, std::forward<Args>(args)...));
    return true;
}

template <typename... Args>
bool Logger::hexdump(LogLevel level, Span<const std::byte> data, std::string_view format, Args&&... args) const
{
    if (level < tag_log_level(tag_))
        return false;

    sink_hexdump(level, data.as_bytes(), fmt::format(format, std::forward<Args>(args)...));
    return true;
}

}  // namespace utils
