#include "kae/matrix.h"

#include <doctest/doctest.h>

namespace kae::test {

TEST_SUITE("matrix")
{
    TEST_CASE("dot")
    {
        Matrix2i a{{1, 2, 3, 4}};
        Matrix2i b{{5, 6, 7, 8}};
        Matrix2i c = a * b;
        CHECK(c == Matrix2i{{19, 22, 43, 50}});
        CHECK(c != b * a);
    }

    TEST_CASE("dot_vec")
    {
        Matrix2i a{{1, 2, 3, 4}};
        Vec2i b{5, 6};
        Vec2i c = a * b;
        CHECK(c.x == 17);
        CHECK(c.y == 39);
    }

    TEST_CASE("constexpr")
    {
        constexpr Matrix2i matrix;
        static_assert(matrix(0, 0) == 0);
    }
}

}  // namespace kae::test
