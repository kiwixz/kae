#pragma once

#include <array>
#include <type_traits>

namespace utils::make_array_detail {

template <typename T, typename... Args>
struct ArrayTypeTrait {
    using Type = std::array<T, sizeof...(Args)>;
};
template <typename... Args>
struct ArrayTypeTrait<void, Args...> {
    using Type = std::array<std::common_type_t<Args...>, sizeof...(Args)>;
};

template <typename T, typename... Args>
using ArrayType = typename ArrayTypeTrait<T, Args...>::Type;

}  // namespace utils::make_array_detail
