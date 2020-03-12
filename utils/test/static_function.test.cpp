#include "utils/static_function.h"

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("static_function")
{
#if !defined(__GNUC__) || defined(__clang__)  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83258
    TEST_CASE("simple")
    {
        {
            constexpr auto f = +[] { return 42; };
            CHECK(StaticFunction<f>{}() == 42);
        }
        {
            constexpr auto f = +[](int i) { return i; };
            CHECK(StaticFunction<f>{}(42) == 42);
        }
        {
            int i = 0;
            constexpr auto f = +[](int& ref) -> int& { return ref; };
            StaticFunction<f>{}(i) = 42;
            CHECK(i == 42);
        }
        {
            constexpr auto f = +[] { return 42; };
            constexpr StaticFunction<f> sf;
            CHECK(std::integral_constant<int, sf()>::value == 42);
        }
        {
            constexpr auto f = +[]() noexcept { return 42; };
            static_assert(std::is_nothrow_invocable_v<StaticFunction<f>>);
        }
    }
#endif
}

}  // namespace utils::test
