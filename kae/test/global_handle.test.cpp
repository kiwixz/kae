#include "kae/global_handle.h"

#include <utility>

#include <doctest/doctest.h>

namespace kae::test {
namespace {

template <typename T>
int& global()
{
    static int c;
    return c;
}

}  // namespace


TEST_SUITE("global_handle")
{
#if !defined(__GNUC__) || defined(__clang__)  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83258
    TEST_CASE("simple")
    {
        int& c = global<struct simple>();
        c = 100;

        constexpr auto init = +[] {
            global<struct simple>() *= 2;
        };
        constexpr auto destroy = +[] {
            global<struct simple>() /= 2;
        };
        using Handle = GlobalHandle<init, destroy>;

        {
            Handle handle;
            CHECK(c == 200);
            Handle handle2 = handle;
            CHECK(c == 200);
            {
                Handle handle3;
                CHECK(c == 200);
            }
            CHECK(c == 200);
            {
                Handle handle3 = handle2;
                CHECK(c == 200);
                handle2 = handle3;
                CHECK(c == 200);
                handle2 = std::move(handle3);
                CHECK(c == 200);
            }
            CHECK(c == 200);
            Handle handle4 = std::move(handle);
            CHECK(c == 200);
        }
        CHECK(c == 100);
        {
            Handle handle;
            CHECK(c == 200);
        }
        CHECK(c == 100);
    }
#endif
}

}  // namespace kae::test
