#include "kae/bind_front.h"

#include <doctest/doctest.h>

namespace kae::test {

#ifndef _WIN32  // would compile with previous MSVC (19.21), but not with current version
TEST_SUITE("bind_front")
{
    TEST_CASE("simple")
    {
        auto add = [](int a, int b) {
            return a + b;
        };
        CHECK(add(1, 2) == 3);
        CHECK(bind_front(add)(1, 2) == 3);
        CHECK(bind_front(add, 1)(2) == 3);
        CHECK(bind_front(add, 1, 2)() == 3);
    }

    TEST_CASE("ref")
    {
        auto ref = [](int& a) -> int& {
            return a;
        };
        int a = 4;
        CHECK(ref(a) == 4);
        CHECK(bind_front(ref)(a) == 4);
        decltype(auto) b = bind_front(ref)(a);
        bind_front(ref)(b) = 5;
        CHECK(bind_front(ref)(a) == 5);
        CHECK(bind_front(ref, std::ref(a))() == 5);
    }
}
#endif

}  // namespace kae::test
