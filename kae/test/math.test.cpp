#include "kae/math.h"

#include <doctest/doctest.h>

namespace kae::test {

TEST_SUITE("math")
{
    TEST_CASE("ceil")
    {
        static_assert(0 == ceil<int>(0.0));
        static_assert(4 == ceil<int>(4.f));
        static_assert(4 == ceil<char>(4.0));
        static_assert(5 == ceil<int>(4.5));
        static_assert(5 == ceil<int>(4.000'000'000'000'01));
        static_assert(5 == ceil<int>(4.999'999'999'999'99));

        static_assert(0 == ceil<int>(-0.0));
        static_assert(-4 == ceil<int>(-4.f));
        static_assert(-4 == ceil<char>(-4.0));
        static_assert(-4 == ceil<int>(-4.5));
        static_assert(-4 == ceil<int>(-4.000'000'000'000'01));
        static_assert(-4 == ceil<int>(-4.999'999'999'999'99));
    }

    TEST_CASE("ceil2")
    {
        CHECK(ceil2(0) == 1);
        CHECK(ceil2(1) == 1);
        CHECK(ceil2(2) == 2);
        CHECK(ceil2(3) == 4);
        CHECK(ceil2(4) == 4);
        CHECK(ceil2(5) == 8);

        CHECK(ceil2(-1) == -1);
        CHECK(ceil2(-2) == -2);
        CHECK(ceil2(-3) == -4);
        CHECK(ceil2(-4) == -4);
        CHECK(ceil2(-5) == -8);
    }

    TEST_CASE("div_ceil")
    {
        static_assert(div_ceil(0, 2) == 0);
        static_assert(div_ceil(1, 2) == 1);
        static_assert(div_ceil(2, 2) == 1);
        static_assert(div_ceil(3, 2) == 2);
        static_assert(div_ceil(4, 2) == 2);
        static_assert(div_ceil(5, 2) == 3);
    }

    TEST_CASE("floor")
    {
        static_assert(0 == floor<int>(0.0));
        static_assert(4 == floor<int>(4.f));
        static_assert(4 == floor<char>(4.0));
        static_assert(4 == floor<int>(4.5));
        static_assert(4 == floor<int>(4.000'000'000'000'01));
        static_assert(4 == floor<int>(4.999'999'999'999'99));

        static_assert(0 == floor<int>(-0.0));
        static_assert(-4 == floor<int>(-4.f));
        static_assert(-4 == floor<char>(-4.0));
        static_assert(-5 == floor<int>(-4.5));
        static_assert(-5 == floor<int>(-4.000'000'000'000'01));
        static_assert(-5 == floor<int>(-4.999'999'999'999'99));
    }
}

}  // namespace kae::test
