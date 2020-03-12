#pragma once

#include <type_traits>

namespace utils {

template <typename... Args>
struct IsPackSame : std::false_type {};

template <typename T>
struct IsPackSame<T, T> : std::true_type {};


template <typename... Args>
inline constexpr bool is_pack_same = IsPackSame<Args...>::value;

}  // namespace utils
