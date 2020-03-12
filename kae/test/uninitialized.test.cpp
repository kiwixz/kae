#include "kae/uninitialized.h"

#include <doctest/doctest.h>

#include "kae/function.h"
#include "kae/function_ref.h"

namespace kae::test {

TEST_SUITE("uninitialized")
{
    TEST_CASE("simple")
    {
        struct S {
            S() = default;

            S(int i) :
                i_{i}
            {}

            S(Function<void()> f) :
                f_{std::move(f)}
            {}

            S(Function<void()> f, FunctionRef<void()> f2) :
                f_{std::move(f)}
            {
                f2();
            }

            ~S()
            {
                if (f_)
                    f_();
            }

            S(const S&) = delete;
            S& operator=(const S&) = delete;
            S(S&&)
            noexcept = delete;
            S& operator=(S&&) noexcept = delete;

            int i()
            {
                return i_;
            }

        private:
            int i_ = 42;
            Function<void()> f_;
        };


        int c = 0;
        Uninitialized<S> s;

        s.emplace();
        CHECK((*s).i() == 42);
        CHECK(s->i() == 42);
        s.destroy();

        s.emplace(64);
        CHECK((*s).i() == 64);
        CHECK(s->i() == 64);
        s.destroy();

        s.emplace([&] { c = 1; });
        CHECK(c == 0);
        s.destroy();
        CHECK(c == 1);

        s.emplace([&] { c = 3; }, [&] { c = 2; });
        CHECK(c == 2);
        s.destroy();
        CHECK(c == 3);
    }
}

}  // namespace kae::test
