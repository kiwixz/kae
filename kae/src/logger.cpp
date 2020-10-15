#include "kae/logger.h"

#include <chrono>
#include <iostream>

#include <fmt/color.h>
#include <fmt/format.h>

#include "kae/exception.h"
#include "kae/os.h"


namespace kae {
namespace {

struct LogLevelInfo {
    std::string_view name;
    fmt::text_style style;
};

constexpr LogLevelInfo log_level_info(LogLevel level)
{
    switch (level) {
    case LogLevel::debug:
        return {"DEBUG", fmt::fg(fmt::terminal_color::cyan)};
    case LogLevel::info:
        return {"INFO", fmt::fg(fmt::terminal_color::green)};
    case LogLevel::warning:
        return {"WARN", fmt::fg(fmt::terminal_color::yellow)};
    case LogLevel::error:
        return {"ERROR", fmt::fg(fmt::terminal_color::red)};
    case LogLevel::fatal:
        return {"FATAL", fmt::emphasis::bold | fmt::fg(fmt::terminal_color::red)};
    case LogLevel::none:
        return {"NONE", fmt::fg(fmt::terminal_color::magenta)};
    }

    throw MAKE_EXCEPTION("log level not handled: {}", level);
}

}  // namespace


Logger::Logger(std::string tag) :
    tag_{std::move(tag)}
{}

bool Logger::operator()(LogLevel level, std::string_view message) const
{
    if (level < tag_log_level(tag_))
        return false;

    sink(level, message);
    return true;
}

bool Logger::hexdump(LogLevel level, std::span<const std::byte> data, std::string_view title) const
{
    if (level < tag_log_level(tag_))
        return false;

    sink_hexdump(level, std::as_bytes(data), title);
    return true;
}

void Logger::sink(LogLevel level, std::string_view message) const
{
    std::chrono::duration<double> time = std::chrono::steady_clock::now().time_since_epoch();
    LogLevelInfo level_info = log_level_info(level);

    std::clog << fmt::format(level_info.style, "[{:.6f}][{:>5}][{}] {}: {}\n",
                             time.count(), level_info.name, thread_id(), tag_, message);
}

void Logger::sink_hexdump(LogLevel level, std::span<const std::byte> data, std::string_view title) const
{
    constexpr size_t kBytesPerLine = 16;

    std::string message = fmt::format("hexdump {} bytes: {}\n", data.size(), title);

    for (size_t offset = 0; offset < data.size(); offset += kBytesPerLine) {
        std::span<const std::byte> line = data.subspan(offset, std::min(kBytesPerLine, data.size() - offset));
        message += fmt::format("  {:08x}  ", offset);  // offset

        for (std::byte c : line)  // hex bytes
            message += fmt::format("{:02x} ", static_cast<unsigned char>(c));

        for (size_t i = 0; i < kBytesPerLine - line.size(); ++i)  // padding
            message += "   ";

        message += ' ';
        for (std::byte c : line)  // ascii bytes
            message += std::isprint(static_cast<unsigned char>(c)) ? static_cast<char>(c) : '.';

        message += '\n';
    }

    return sink(level, message);
}

}  // namespace kae
