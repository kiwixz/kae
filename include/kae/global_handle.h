#pragma once

#include <atomic>

namespace kae {

template <auto Tinit, auto Tdestroy>
struct GlobalHandle {
    static constexpr auto init = Tinit;
    static constexpr auto destroy = Tdestroy;

    GlobalHandle();
    ~GlobalHandle();
    GlobalHandle(const GlobalHandle& other);
    GlobalHandle& operator=(const GlobalHandle& other) = default;
    GlobalHandle(GlobalHandle&& other) noexcept;
    GlobalHandle& operator=(GlobalHandle&& other) noexcept = default;

private:
    static std::atomic<int>& counter();
};


template <auto Tinit, auto Tdestroy>
GlobalHandle<Tinit, Tdestroy>::GlobalHandle()
{
    std::atomic<int>& c = counter();
    if (c.fetch_add(1) == 0)  // was the first
        Tinit();              // msvc doesnt like init();
}

template <auto Tinit, auto Tdestroy>
GlobalHandle<Tinit, Tdestroy>::~GlobalHandle()
{
    std::atomic<int>& c = counter();
    if (c.fetch_sub(1) == 1)  // was the last
        Tdestroy();           // msvc doesnt like destroy();
}

template <auto Tinit, auto Tdestroy>
GlobalHandle<Tinit, Tdestroy>::GlobalHandle(const GlobalHandle& /*other*/) :
    GlobalHandle{}
{}

template <auto Tinit, auto Tdestroy>
GlobalHandle<Tinit, Tdestroy>::GlobalHandle(GlobalHandle&& /*other*/) noexcept :
    GlobalHandle{}
{}

template <auto Tinit, auto Tdestroy>
std::atomic<int>& GlobalHandle<Tinit, Tdestroy>::counter()
{
    static std::atomic<int> counter = 0;
    return counter;
}

}  // namespace kae
