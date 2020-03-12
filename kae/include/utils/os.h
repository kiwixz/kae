#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace kae {

using ThreadId = unsigned long long;

struct StackFrame {
    void* address = nullptr;
    std::string module;
    std::string function;
    std::string file;
    int line = 0;
};


std::filesystem::path app_directory(std::string_view app_name);

ThreadId thread_id();

std::string thread_name();
void set_thread_name(const std::string& name);

std::vector<StackFrame> stacktrace();

}  // namespace kae
