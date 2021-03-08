#pragma once

#include <cmath>
#include <type_traits>

#include "kae/pp.h"

namespace kae {

template <typename Unit, typename Arg>
constexpr Unit ceil(Arg a)
{
    static_assert(std::is_integral_v<Unit> && std::is_floating_point_v<Arg>);

    Unit unit_a = static_cast<Unit>(a);
    if (a == static_cast<Arg>(unit_a))
        return unit_a;
    return unit_a + (a > 0);
}

template <typename T>
constexpr T ceil2(T a)
{
    static_assert(std::is_integral_v<T>);

    if (a < 0)
        return -ceil2(-a);

    if (a <= 1)
        return 1;

    return static_cast<T>(1u << static_cast<unsigned>(std::log2(a - 1) + 1));
}

template <typename T>
constexpr T div_ceil(T a, T unit)
{
    static_assert(std::is_integral_v<T>);
    ASSERT(a >= 0 && unit > 0);

    return (a + unit - 1) / unit;
}

template <typename Unit, typename Arg>
constexpr Unit floor(Arg a)
{
    static_assert(std::is_integral_v<Unit> && std::is_floating_point_v<Arg>);

    Unit unit_a = static_cast<Unit>(a);
    if (a == static_cast<Arg>(unit_a))
        return unit_a;
    return unit_a - (a < 0);
}

template <typename Unit, typename Arg>
constexpr Unit trunc(Arg a)
{
    return a > 0 ? floor<Unit>(a) : ceil<Unit>(a);
}

template <typename Unit, typename Arg>
constexpr Unit ntrunc(Arg a)
{
    return a > 0 ? ceil<Unit>(a) : floor<Unit>(a);
}

template <typename T>
constexpr T pow2(T a)
{
    return a * a;
}

}  // namespace kae
