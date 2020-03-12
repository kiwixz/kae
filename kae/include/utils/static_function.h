#pragma once

#include <type_traits>
#include <utility>

namespace kae {

template <auto Tfunction>
struct StaticFunction {
    static constexpr auto function = Tfunction;

    using Function = decltype(function);

    template <typename... Args>
    constexpr std::invoke_result_t<Function, Args...> operator()(Args&&... args)
            const noexcept(std::is_nothrow_invocable_v<Function, Args...>);
};

template <auto Tfunction>
template <typename... Args>
constexpr std::invoke_result_t<typename StaticFunction<Tfunction>::Function, Args...>
StaticFunction<Tfunction>::operator()(Args&&... args)
        const noexcept(std::is_nothrow_invocable_v<Function, Args...>)
{
    return function(std::forward<Args>(args)...);
}

}  // namespace kae
