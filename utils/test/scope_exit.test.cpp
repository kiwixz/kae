#include "utils/scope_exit.h"

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("scope_exit")
{
    TEST_CASE("simple")
    {
        bool pass = false;
        {
            ScopeExit scope_exit{[&] { pass = true; }};
            CHECK(!pass);
        }
        CHECK(pass);
    }

    TEST_CASE("release")
    {
        int c = 0;
        {
            ScopeExit scope_exit{[&] { ++c; }};
            CHECK(c == 0);
            scope_exit.release()();
            CHECK(c == 1);
        }
        CHECK(c == 1);
    }

    TEST_CASE("reassign")
    {
        int c = 0;
        {
            ScopeExit scope_exit{[&] { ++c; }};
            CHECK(c == 0);
            scope_exit = ScopeExit{[&] { c += 10; }};
            CHECK(c == 1);
        }
        CHECK(c == 11);
    }

    TEST_CASE("empty")
    {
        ScopeExit{};
    }
}

}  // namespace utils::test
