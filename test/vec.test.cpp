#include "kae/vec.h"

#include <doctest/doctest.h>

namespace kae::test {

TEST_SUITE("vec")
{
    TEST_CASE("constexpr")
    {
        constexpr Vec2i vec{1, 2};
        static_assert(vec.x + vec.y == 3);
    }
}

}  // namespace kae::test
