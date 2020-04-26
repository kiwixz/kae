#pragma once

#include <cstdlib>
#include <filesystem>
#include <type_traits>
#include <unordered_map>

#include "kae/exception.h"

namespace kae {

struct Config {
    bool contains(const std::string& key) const;
    std::string dump(std::string_view prefix) const;
    const std::string& get_raw(const std::string& key) const;
    void show_help(std::string_view app_name, std::string_view pos_args = "") const;

    template <typename T>
    T get(const std::string& key) const;

    template <typename T>
    void get_to(const std::string& key, T& value) const;

    void remove(const std::string& key);
    void clear();

    /// Parse command-line arguments (and config file if explicitly given).
    /// Parsed arguments will be removed.
    /// Return true if application should show help (all arguments may not be parsed, but you can call again).
    bool parse_args(int& argc, char** argv, bool allow_unknown = false);

    void parse_global_config(std::string_view app_name, bool allow_unknown = false);
    void parse_file(const std::filesystem::path& path, bool allow_unknown = false);
    void parse_file_content(std::string_view content, bool allow_unknown = false);

    template <typename T>
    void set(std::string key, T&& value);

private:
    std::unordered_map<std::string, std::string> options_;

    void set_parsed_option(std::string key, std::string value, bool allow_unknown);
};


template <typename T>
T Config::get(const std::string& key) const
{
    const std::string& value = get_raw(key);
    if constexpr (std::is_same_v<T, bool>) {
        if (value == "true" || value == "1")
            return true;
        if (value == "false" || value == "0")
            return false;
        throw MAKE_EXCEPTION("key '{}': expected boolean value, got '{}'", key, value);
    }
    else if constexpr (std::is_arithmetic_v<T>) {
        if (value.length() == 0)
            throw MAKE_EXCEPTION("key '{}': expected numeric value, got empty string", key);

        char* end = nullptr;
        T result;
        if constexpr (std::is_integral_v<T>) {
            if constexpr (std::is_unsigned_v<T>)
                result = static_cast<T>(std::strtoull(value.c_str(), &end, 0));
            else
                result = static_cast<T>(std::strtoll(value.c_str(), &end, 0));
        }
        else
            result = static_cast<T>(std::strtold(value.c_str(), &end));

        if (end != value.c_str() + value.length())
            throw MAKE_EXCEPTION("key '{}': expected {} value, got '{}'",
                                 key, std::is_integral_v<T> ? "integer" : "floating-point", value);
        return result;
    }
    else
        return T{value};
}

template <typename T>
void Config::get_to(const std::string& key, T& value) const
{
    value = get<T>(key);
}

template <typename T>
void Config::set(std::string key, T&& value)
{
    if constexpr (std::is_same_v<T, bool>)
        return set(std::move(key), value ? "true" : "false");
    else if constexpr (std::is_arithmetic_v<T>)
        return set(std::move(key), std::to_string(value));
    else
        options_.insert_or_assign(std::move(key), std::forward<T>(value));
}

}  // namespace kae
