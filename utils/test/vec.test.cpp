#include "utils/vec.h"

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("vec")
{
    TEST_CASE("constexpr")
    {
        constexpr Vec2i vec{1, 2};
        static_assert(vec.x + vec.y == 3);
    }
}

}  // namespace utils::test
