#include "utils/circular.h"

#include <mutex>

#include <doctest/doctest.h>

#include "utils/scope_exit.h"

namespace utils::test {

TEST_SUITE("circular")
{
    TEST_CASE("simple")
    {
        Circular<int, 3> c;
        CHECK(c.max_size() == 3);

        auto eq = [&](const std::initializer_list<int>& list) {
            const Circular<int, 3>& const_c = c;

            if (list.size() == 0) {
                CHECK(const_c.empty());
                CHECK(const_c.size() == 0);
                return;
            }

            CHECK(!const_c.empty());
            CHECK(const_c.size() == list.size());
            CHECK(const_c.front() == *list.begin());
            CHECK(const_c.back() == *(list.end() - 1));

            auto it = list.begin();
            for (int i : const_c) {
                CHECK(i == *it);
                ++it;
            }
        };

        eq({});
        c.push_back(1);
        eq({1});
        c.push_back(2);
        eq({1, 2});
        c.push_front(0);
        eq({0, 1, 2});
        c.push_back(3);
        eq({1, 2, 3});
        c.push_back(4);
        eq({2, 3, 4});
        c.push_front(1);
        eq({1, 2, 3});

        c.pop_front();
        eq({2, 3});
        c.push_front(1);
        eq({1, 2, 3});
        c.pop_front();
        eq({2, 3});
        c.push_back(4);
        eq({2, 3, 4});

        c.pop_back();
        eq({2, 3});
        c.push_back(4);
        eq({2, 3, 4});
        c.pop_back();
        eq({2, 3});
        c.push_front(1);
        eq({1, 2, 3});

        c.push_back(4);
        eq({2, 3, 4});
        c.push_back(5);
        eq({3, 4, 5});
        c.push_back(6);
        eq({4, 5, 6});
        c.push_back(7);
        eq({5, 6, 7});
        c.push_back(8);
        eq({6, 7, 8});

        c.pop_front();
        eq({7, 8});
        c.pop_front();
        eq({8});
        c.pop_front();
        eq({});
    }

    TEST_CASE("strict_lifetime")
    {
        int r = 42;
        Circular<ScopeExit, 3> c;

        c.push_back(ScopeExit{[&] { r = 0; }});
        c.pop_back();
        CHECK(r == 0);

        c.push_back(ScopeExit{[&] { r += 8; }});
        CHECK(r == 0);
        c.push_front(ScopeExit{[&] { r += 4; }});
        CHECK(r == 0);
        c.push_front(ScopeExit{[&] { r += 2; }});
        CHECK(r == 0);
        c.push_back(ScopeExit{[&] { r += 32; }});
        CHECK(r == 2);
        c.push_back(ScopeExit{[&] { r += 16; }});
        CHECK(r == 2 + 4);
        c.pop_front();
        CHECK(r == 2 + 4 + 8);
        c.pop_back();
        CHECK(r == 2 + 4 + 8 + 16);
        c.pop_back();
        CHECK(r == 2 + 4 + 8 + 16 + 32);
    }

    TEST_CASE("move_copy")
    {
        auto eq = [](const Circular<int, 3>& circular, const std::initializer_list<int>& list) {
            CHECK(circular.size() == list.size());
            auto it = list.begin();
            for (int i : circular) {
                CHECK(i == *it);
                ++it;
            }
        };

        auto eqp = [](const Circular<std::unique_ptr<int>, 3>& circular, const std::initializer_list<int>& list) {
            CHECK(circular.size() == list.size());
            auto it = list.begin();
            for (const std::unique_ptr<int>& i : circular) {
                CHECK(*i == *it);
                ++it;
            }
        };

        Circular<int, 3> c;
        eq(c, {});
        c.push_back(1);
        eq(c, {1});
        c.push_back(2);
        eq(c, {1, 2});
        c.push_back(3);
        eq(c, {1, 2, 3});

        Circular<int, 3> c2 = c;
        eq(c2, {1, 2, 3});

        Circular<int, 3> c3 = std::move(c2);
        eq(c3, {1, 2, 3});

        Circular<std::unique_ptr<int>, 3> cp;
        eqp(cp, {});
        cp.push_back(std::make_unique<int>(1));
        eqp(cp, {1});
        cp.push_back(std::make_unique<int>(2));
        eqp(cp, {1, 2});
        cp.push_back(std::make_unique<int>(3));
        eqp(cp, {1, 2, 3});

        Circular<std::unique_ptr<int>, 3> cp2 = std::move(cp);
        eqp(cp2, {1, 2, 3});
    }

    TEST_CASE("leaks")
    {
        int r = 0;

        {
            Circular<ScopeExit, 3> c;
            c.push_back(ScopeExit{[&] { ++r; }});
            CHECK(r == 0);
            c.push_back(ScopeExit{[&] { ++r; }});
            CHECK(r == 0);
            c.push_back(ScopeExit{[&] { ++r; }});
            CHECK(r == 0);
            c.push_back(ScopeExit{[&] { ++r; }});
            CHECK(r == 1);
            c.push_back(ScopeExit{[&] { ++r; }});
            CHECK(r == 2);
        }

        CHECK(r == 5);
    }

    TEST_CASE("unmovable_objects")
    {
        Circular<std::mutex, 3> c;

        c.emplace_back();
        c.emplace_front();
        c.emplace_back();
        c.emplace_back();
        c.emplace_back();
        c.pop_back();
        c.pop_back();
    }
}

}  // namespace utils::test
