#include "kae/thread_pool.h"

#include <atomic>
#include <memory>

#include <doctest/doctest.h>

namespace kae::test {

TEST_SUITE("thread_pool")
{
    TEST_CASE("submit")
    {
        ThreadPool pool{8};

        CHECK(pool.submit([] { return std::string{"hello"}; }).get() == "hello");

        {
            bool pass = false;
            pool.submit([&] {
                    pass = true;
                })
                    .get();
            CHECK(pass);
        }

        {
            bool pass = false;
            pool.submit([](bool& b) {
                    b = true;
                },
                        std::ref(pass))
                    .get();
            CHECK(pass);
        }
    }

    TEST_CASE("submit_moveonly")
    {
        ThreadPool pool{8};

        pool.submit([ptr = std::make_unique<int>()] {});
        pool.submit([ptr = std::make_shared<int>()]() mutable {
            ptr.reset();
        });
    }

    TEST_CASE("extend")
    {
        ThreadPool pool;

        std::atomic<int> c = 0;
        auto f0 = pool.submit([&] {
            ++c;
        });
        auto f1 = pool.submit([&] {
            ++c;
        });

        pool.extend(1);

        f0.get();
        f1.get();

        auto f2 = pool.submit([&] {
            ++c;
        });
        auto f3 = pool.submit([&] {
            ++c;
        });

        f2.get();
        f3.get();

        CHECK(c == 4);
    }
}

}  // namespace kae::test
