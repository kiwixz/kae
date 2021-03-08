#pragma once

#include "kae/make_array_detail.h"

namespace kae {

/// If T is void, it will be deduced from arguments
template <typename T = void, typename... Args>
constexpr make_array_detail::ArrayType<T, Args...> make_array(Args&&... args);


template <typename T, typename... Args>
constexpr make_array_detail::ArrayType<T, Args...> make_array(Args&&... args)
{
    return {std::forward<Args>(args)...};
}

}  // namespace kae
