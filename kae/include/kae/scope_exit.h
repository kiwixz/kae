#pragma once

#include "kae/function.h"

namespace kae {

/// Classic RAII wrapper.
struct [[nodiscard]] ScopeExit {
    ScopeExit() = default;
    explicit ScopeExit(std::function<void()> function);
    ~ScopeExit();
    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;
    ScopeExit(ScopeExit&& other) noexcept;
    ScopeExit& operator=(ScopeExit&& other) noexcept;

    kae::UniqueFunction<void()> release();

private:
    kae::UniqueFunction<void()> function_;
};

}  // namespace kae
