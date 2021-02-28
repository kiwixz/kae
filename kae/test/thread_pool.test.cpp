#include "kae/thread_pool.h"

#include <atomic>
#include <memory>

#include <doctest/doctest.h>

namespace kae::test {

TEST_SUITE("thread_pool")
{
    TEST_CASE("submit")
    {
        ThreadPool pool{4};

        CHECK(pool.submit([] { return std::string{"hello"}; }).get() == "hello");
        CHECK(pool.submit([](const std::string& a) { return a; }, "world").get() == "world");

        pool.submit([ptr = std::make_unique<int>()] {});
        CHECK(*pool.submit([ptr = std::make_unique<int>()]() mutable -> std::unique_ptr<int> {
                       *ptr = 72;
                       return std::move(ptr);
                   }).get()
              == 72);
        CHECK(*pool.submit([ptr = std::make_shared<int>()]() mutable {
                       *ptr = 91;
                       return ptr;
                   }).get()
              == 91);
        pool.submit([ptr = std::make_shared<int>()]() mutable {
            ptr.reset();
        });

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

        std::this_thread::sleep_for(std::chrono::milliseconds{10});
        CHECK(c == 0);

        pool.extend(1);

        f0.get();
        f1.get();

        CHECK(c == 2);

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
