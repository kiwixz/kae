#include "kae/function_ref.h"

#include <doctest/doctest.h>

namespace kae::test {

TEST_SUITE("function_ref")
{
    TEST_CASE("simple")
    {
        {
            kae::FunctionRef<void()> fref;
            fref = [] {};
            fref();
        }

        CHECK(kae::FunctionRef<int()>{[] { return 42; }}() == 42);

        {
            auto f = [](int i) { return i; };
            CHECK(kae::FunctionRef<int(int)>{f}(42) == 42);
            CHECK(kae::FunctionRef<int()>{std::bind(f, 42)}() == 42);
        }
        {
            int i = 0;
            auto f = [](int& ref) -> int& { return ref; };
            kae::FunctionRef<int&(int&)>{f}(i) = 42;
            CHECK(i == 42);
        }
    }

#if !defined(__GNUC__) || defined(__clang__)  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83258
    TEST_CASE("function_pointers")
    {
        kae::FunctionRef<void()>{+[] {}}();
        CHECK(kae::FunctionRef<int()>{+[] { return 42; }}() == 42);
        CHECK(kae::FunctionRef<int(int)>{+[](int i) { return i; }}(42) == 42);

        {
            int i = 0;
            kae::FunctionRef<int&(int&)>{+[](int& ref) -> int& { return ref; }}(i) = 42;
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

        CHECK(kae::FunctionRef<int(S*)>{&S::foo}(&s) == 42);
        CHECK(kae::FunctionRef<int(int)>{&S::bar}(42) == 42);
    }
}

}  // namespace kae::test
