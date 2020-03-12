#include "kae/config.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include <fmt/printf.h>

#include "kae/os.h"

namespace kae {
namespace {

constexpr std::string_view implicit_value = "true";

}


bool Config::contains(const std::string& key) const
{
    return options_.count(key) > 0;
}

std::string Config::dump(std::string_view prefix) const
{
    std::vector<std::pair<std::string, std::string>> options(options_.begin(), options_.end());
    std::sort(options.begin(), options.end());
    std::string output;
    for (const std::pair<std::string, std::string>& option : options)
        output += fmt::format("{}{}={}\n", prefix, option.first, option.second);
    return output;
}

const std::string& Config::get_raw(const std::string& key) const
{
    return options_.at(key);
}

void Config::show_help(std::string_view app_name, std::string_view pos_args) const
{
    fmt::print("Usage: {} [--help] [+config_file] [-option[=value]...]", app_name);
    if (!pos_args.empty())
        fmt::print(" [--] {}", pos_args);
    fmt::print("\nCurrent config:\n{}", dump("\t"));
}

void Config::remove(const std::string& key)
{
    if (options_.erase(key) == 0)
        throw MAKE_EXCEPTION("key '{}' missing", key);
}

void Config::clear()
{
    options_.clear();
}

bool Config::parse_args(int& argc, char** argv, bool allow_unknown)
{
    std::vector<int> used_args;
    used_args.reserve(argc - 1);

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            used_args.push_back(i);
            std::string_view arg = argv[i] + 1;
            if (arg == "-")
                break;
            if (arg == "-help")
                return true;

            size_t eq = arg.find('=');
            std::string key{arg.substr(0, eq)};
            std::string value;
            if (eq == std::string::npos)
                value = implicit_value;
            else
                value = std::string{arg.substr(eq + 1)};
            set_parsed_option(std::move(key), std::move(value), allow_unknown);
        }
        else if (argv[i][0] == '+') {
            used_args.push_back(i);
            parse_file(argv[i] + 1, allow_unknown);
        }
    }

    if (used_args.empty())
        return false;

    argc -= static_cast<int>(used_args.size());
    int old_index = used_args[0] + 1;
    int new_index = used_args[0];
    for (size_t i = 1; i < used_args.size(); ++i) {
        for (; old_index < used_args[i]; ++old_index, ++new_index)
            argv[new_index] = argv[old_index];
        ++old_index;
    }
    for (; new_index <= argc; ++old_index, ++new_index)
        argv[new_index] = argv[old_index];

    return false;
}

void Config::parse_global_config(std::string_view app_name, bool allow_unknown)
{
    auto parse_if_exists = [&](const std::filesystem::path& path) {
        if (std::filesystem::exists(path))
            parse_file(path, allow_unknown);
    };

    static constexpr std::string_view file_name = "config.ini";
    parse_if_exists(kae::app_directory(app_name) / file_name);
    parse_if_exists(file_name);
}

void Config::parse_file(const std::filesystem::path& path, bool allow_unknown)
{
    std::ifstream ifs{path, std::ios::ate};
    if (!ifs)
        throw MAKE_EXCEPTION("could not open config file '{}'", path.generic_string());
    std::streamoff size = ifs.tellg();
    std::string content(size, '\0');
    ifs.seekg(0, std::ios::beg);
    ifs.read(content.data(), size);
    ifs.close();
    parse_file_content(content, allow_unknown);
}

void Config::parse_file_content(std::string_view content, bool allow_unknown)
{
    const char* ptr = content.data();
    const char* end = content.data() + content.size();
    auto is_endline = [&](const char* c) {
        return c >= end || *c == '\n' || *c == '\r';
    };
    auto is_blank = [&](char c) {
        return c == ' ' || c == '\t';
    };

    std::string prefix;
    while (ptr < end) {
        while (ptr < end && (is_endline(ptr) || is_blank(*ptr)))
            ++ptr;
        if (ptr >= end)
            break;

        if (*ptr == '#') {  // comment
            while (!is_endline(ptr))
                ++ptr;
            continue;
        }

        if (*ptr == '[') {  // section header
            ++ptr;
            while (!is_endline(ptr) && is_blank(*ptr))  // skip blanks at beginning
                ++ptr;
            const char* section_begin = ptr;
            while (!(is_endline(ptr) || *ptr == ']'))
                ++ptr;
            if (ptr >= end)
                throw MAKE_EXCEPTION("config has mismatched '['");
            const char* section_end = ptr;
            while (section_end > section_begin && is_blank(section_end[-1]))  // trim blanks at end
                --section_end;

            std::string section = {section_begin, section_end};
            if (section.empty())
                prefix = {};
            else if (section[0] == '.')
                prefix += section.substr(1) + '.';
            else
                prefix = std::move(section) + '.';
            ++ptr;
            continue;
        }

        const char* key_begin = ptr;
        while (!(is_endline(ptr) || *ptr == '='))
            ++ptr;
        const char* key_end = ptr;
        while (key_end > key_begin && is_blank(key_end[-1]))  // trim blanks at end
            --key_end;
        std::string key{key_begin, key_end};

        std::string value;
        if (ptr == end)
            value = implicit_value;
        else {  // *ptr is '='
            ++ptr;
            if (ptr < end) {
                if (is_blank(*ptr))  // skip only one blank (so you can start value with blanks without escaping)
                    ++ptr;
                const char* value_begin = ptr;
                while (!is_endline(ptr))
                    ++ptr;
                const char* value_end = ptr;
                value = {value_begin, value_end};
            }
        }

        set_parsed_option(prefix + std::move(key), std::move(value), allow_unknown);
    }
}

void Config::set_parsed_option(std::string key, std::string value, bool allow_unknown)
{
    auto it = options_.find(key);
    if (it == options_.end()) {
        if (allow_unknown)
            options_.insert({std::move(key), std::move(value)});
        else
            throw MAKE_EXCEPTION("unknown option '{}'", key);
    }
    else
        it->second = std::move(value);
}

}  // namespace kae
