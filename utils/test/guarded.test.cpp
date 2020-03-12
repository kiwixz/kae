#include "utils/guarded.h"

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("guarded")
{
    TEST_CASE("simple")
    {
        Guarded<int> c{10};

        {
            Guarded<int>::Handle a = c.lock();
            CHECK(*a == 10);
            CHECK(!c.try_lock());
            *a = 11;
        }

        {
            Guarded<int>::Handle a = c.lock();
            CHECK(*a == 11);
            CHECK(!c.try_lock());
        }
    }
}

}  // namespace utils::test
