#pragma once

#include <cstring>
#include <type_traits>

namespace utils {

template <typename Result, typename Arg>
Result bit_cast(const Arg& src) noexcept
{
    static_assert(sizeof(Arg) == sizeof(Result));
    static_assert(std::is_trivially_copyable_v<Arg>);
    static_assert(std::is_trivial_v<Result>);

    Result dst;
    std::memcpy(&dst, &src, sizeof(Result));
    return dst;
}

}  // namespace utils
