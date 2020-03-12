#include "kae/scope_exit.h"

namespace kae {

ScopeExit::ScopeExit(std::function<void()> function) :
    function_{std::move(function)}
{}

ScopeExit::~ScopeExit()
{
    if (!function_)
        return;
    function_();
}

ScopeExit::ScopeExit(ScopeExit&& other) noexcept
{
    *this = std::move(other);
}

ScopeExit& ScopeExit::operator=(ScopeExit&& other) noexcept
{
    if (function_)
        function_();
    function_ = std::move(other.function_);
    other.function_ = {};
    return *this;
}

std::function<void()> ScopeExit::release()
{
    std::function<void()> f = std::move(function_);
    function_ = {};
    return f;
}

}  // namespace kae
