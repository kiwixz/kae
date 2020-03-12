#include "utils/function.h"

#include <functional>
#include <memory>

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("function")
{
    TEST_CASE("simple")
    {
        {
            utils::Function<void()> fref;
            fref = [] {};
            fref();
        }

        CHECK(utils::Function<int()>{[] { return 42; }}() == 42);

        {
            auto f = [](int i) { return i; };
            CHECK(utils::Function<int(int)>{f}(42) == 42);
            CHECK(utils::Function<int()>{std::bind(f, 42)}() == 42);
        }
        {
            int i = 0;
            auto f = [](int& ref) -> int& { return ref; };
            utils::Function<int&(int&)>{f}(i) = 42;
            CHECK(i == 42);
        }
    }

#if !defined(__GNUC__) || defined(__clang__)  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83258
    TEST_CASE("function_pointers")
    {
        utils::Function<void()>{+[] {}}();
        CHECK(utils::Function<int()>{+[] { return 42; }}() == 42);
        CHECK(utils::Function<int(int)>{+[](int i) { return i; }}(42) == 42);

        {
            int i = 0;
            utils::Function<int&(int&)>{+[](int& ref) -> int& { return ref; }}(i) = 42;
            CHECK(i == 42);
        }
    }
#endif

    TEST_CASE("member_pointers")
    {
        struct S {
            int foo() const
            {
                return 42;
            }

            static int bar(int i)
            {
                return i;
            }
        };

        S s;

        CHECK(utils::Function<int(S*)>{&S::foo}(&s) == 42);
        CHECK(utils::Function<int(int)>{&S::bar}(42) == 42);
    }

    TEST_CASE("unique")
    {
        {
            utils::UniqueFunction<int()> f;
            f = [ptr = std::make_unique<int>(42)] {
                return ptr ? *ptr : 0xBAD;
            };
            CHECK(f() == 42);

            utils::UniqueFunction<int()> f2 = std::move(f);
            CHECK(f() == 0xBAD);
            CHECK(f2() == 42);
        }

        utils::UniqueFunction<int(std::unique_ptr<int> &&)> f = [ptr = std::make_unique<int>(42)](std::unique_ptr<int>&& new_ptr) mutable {
            int i = *ptr;
            ptr = std::move(new_ptr);
            return i;
        };
        CHECK(f(std::make_unique<int>(84)) == 42);
        CHECK(f({}) == 84);
    }
}

}  // namespace utils::test
