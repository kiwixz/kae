#pragma once

#include <array>
#include <cstdint>

namespace kae {

struct Rand64 {
    Rand64();

    uint64_t operator()();

private:
    std::array<uint64_t, 4> state_;
};


struct RandF {
    RandF();

    double operator()();

private:
    std::array<uint64_t, 4> state_;
};

}  // namespace kae
