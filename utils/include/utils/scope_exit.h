#pragma once

#include <functional>

namespace utils {

/// Classic RAII wrapper.
struct [[nodiscard]] ScopeExit
{
    ScopeExit() = default;
    explicit ScopeExit(std::function<void()> function);
    ~ScopeExit();
    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;
    ScopeExit(ScopeExit && other) noexcept;
    ScopeExit& operator=(ScopeExit&& other) noexcept;

    std::function<void()> release();

private:
    std::function<void()> function_;
};

}  // namespace utils
