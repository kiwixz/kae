#include "utils/any.h"

#include <memory>

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("any")
{
    TEST_CASE("simple")
    {
        Any a;
        CHECK(!a);
        a = 42;
        CHECK(a.as<int>() == 42);
        a = 42.0;
        CHECK(a.as<double>() == 42.0);

        Any b = a;
        a = 54;
        CHECK(a.as<int>() == 54);
        CHECK(b.as<double>() == 42.0);

        b = std::move(a);
        CHECK(b.as<int>() == 54);
    }

    TEST_CASE("unique")
    {
        UniqueAny a;
        CHECK(!a);
        a = std::make_unique<int>(42);
        CHECK(*a.as<std::unique_ptr<int>>() == 42);
        a = std::make_unique<double>(42.0);
        CHECK(*a.as<std::unique_ptr<double>>() == 42.0);

        UniqueAny b = std::move(a);
        CHECK(!a.as<std::unique_ptr<double>>());
        a = std::make_unique<int>(54);
        CHECK(*a.as<std::unique_ptr<int>>() == 54);
        CHECK(*b.as<std::unique_ptr<double>>() == 42.0);
    }
}

}  // namespace utils::test
