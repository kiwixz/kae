#include "kae/resource_manager.h"

#include <doctest/doctest.h>

namespace kae::test {

TEST_SUITE("resource_manager")
{
    TEST_CASE("simple")
    {
        ResourceManager manager;
        CHECK_THROWS((void)manager.get<int>("a"));
        ResourceHandle<int> handle = manager.get<int>("a", [] { return std::make_shared<int>(4); });
        CHECK(*handle == 4);
        manager.get_to("a", handle, [] { return std::make_shared<int>(6); });
        CHECK(*handle == 4);
        {
            ResourceHandle<int> handle_a = handle;
            const_cast<int&>(*handle_a) = 8;
            CHECK(*handle == 8);
            manager.get_to("b", handle, [] { return std::make_shared<int>(6); });
            CHECK(*handle == 6);
            CHECK(*handle_a == 8);
            manager.get_to("a", handle);
            CHECK(*handle == 8);
        }
        CHECK_THROWS((void)manager.get<int>("b"));
    }
}

}  // namespace kae::test
