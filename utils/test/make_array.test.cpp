#include "utils/make_array.h"

#include <array>
#include <string>
#include <type_traits>

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("make_array")
{
    TEST_CASE("simple")
    {
        constexpr auto a = make_array(1.0, 2.0, 3.0);
        static_assert(std::is_same_v<decltype(a), const std::array<double, 3>>);
        static_assert(a[0] == 1.0);
        static_assert(a[1] == 2.0);
        static_assert(a[2] == 3.0);

        auto b = make_array<std::string>("hello", "world");
        static_assert(std::is_same_v<decltype(b), std::array<std::string, 2>>);
        CHECK(b[0].length() == 5);
        CHECK(b[1].length() == 5);
    }
}

}  // namespace utils::test
