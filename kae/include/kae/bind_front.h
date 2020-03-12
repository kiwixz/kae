#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace kae {

template <typename F, typename... Args>
constexpr auto bind_front(F&& f, Args&&... args);


/// Do not forget to wrap references in std::ref when appropriate.
template <typename F, typename... Args>
constexpr auto bind_front(F&& f, Args&&... args)
{
    return [callable = std::forward<F>(f),
            bound_args_tuple = std::make_tuple(std::forward<Args>(args)...)](auto&&... call_args) -> decltype(auto) {
        return std::apply([&](auto&&... bound_args) -> decltype(auto) {
            return std::invoke(callable,
                               std::forward<decltype(bound_args)>(bound_args)...,
                               std::forward<decltype(call_args)>(call_args)...);
        },
                          decltype(bound_args_tuple){bound_args_tuple});  // force a copy so we can call it multiple times
    };
}

}  // namespace kae
