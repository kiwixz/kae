#include "kae/logging.h"

#include <mutex>
#include <shared_mutex>
#include <unordered_map>

namespace kae {

namespace {

std::shared_mutex& logging_mutex_static()
{
    static std::shared_mutex logging_mutex;
    return logging_mutex;
}

LogLevel& global_log_level_static()
{
    static LogLevel level = LogLevel::default_level;
    return level;
}

std::unordered_map<std::string, LogLevel>& tags_log_level_static()
{
    static std::unordered_map<std::string, LogLevel> data;
    return data;
}

}  // namespace


LogLevel global_log_level()
{
    std::shared_lock lock{logging_mutex_static()};
    return global_log_level_static();
}

LogLevel tag_log_level(std::string_view tag)
{
    std::shared_lock lock{logging_mutex_static()};
    std::unordered_map<std::string, LogLevel>& data = tags_log_level_static();
    auto it = data.find(std::string{tag});
    return (it != data.end() ? it->second : global_log_level_static());
}

void set_global_log_level(LogLevel level)
{
    std::unique_lock lock{logging_mutex_static()};
    global_log_level_static() = level;
}

void set_tag_log_level(std::string_view tag, LogLevel level)
{
    std::unique_lock lock{logging_mutex_static()};
    tags_log_level_static().insert_or_assign(std::string{tag}, level);
}

void reset_tag_log_level(std::string_view tag)
{
    std::unique_lock lock{logging_mutex_static()};
    tags_log_level_static().erase(std::string{tag});
}

}  // namespace kae
