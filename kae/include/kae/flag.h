#pragma once

#include <type_traits>

namespace kae {

template <typename>
struct IsFlag : std::false_type {};

template <typename T>
inline constexpr bool is_flag = IsFlag<T>::value;


template <typename TNative>
struct Flag {
    using Native = TNative;
    using Underlying = std::underlying_type_t<Native>;

    Flag() = default;

    Flag(Native native) :
        native_{native}
    {}

    explicit Flag(Underlying underlying) :
        native_{static_cast<Native>(underlying)}
    {}

    explicit operator bool()
    {
        return static_cast<Underlying>(native_) != 0;
    }

    operator Native()
    {
        return native_;
    }

    explicit operator Underlying()
    {
        return static_cast<Underlying>(native_);
    }

private:
    Native native_{};
};

}  // namespace kae


// operators need to be in global namespace to be found

template <typename T, typename std::enable_if_t<kae::is_flag<T>, int> = 0>
kae::Flag<T> operator~(T a)
{
    return ~static_cast<std::underlying_type_t<T>>(a);
}

#define DEF_OP(op)                                                                           \
    template <typename T, typename std::enable_if_t<kae::is_flag<T>, int> = 0>             \
    T& operator op##=(T& a, T b)                                                             \
    {                                                                                        \
        using Underlying = std::underlying_type_t<T>;                                        \
        return a = static_cast<T>(static_cast<Underlying>(a) op static_cast<Underlying>(b)); \
    }                                                                                        \
                                                                                             \
    template <typename T, typename std::enable_if_t<kae::is_flag<T>, int> = 0>             \
    kae::Flag<T> operator op(T a, T b)                                                     \
    {                                                                                        \
        using Underlying = std::underlying_type_t<T>;                                        \
        return kae::Flag<T>{static_cast<Underlying>(a) op static_cast<Underlying>(b)};     \
    }

DEF_OP(&)
DEF_OP(|)
DEF_OP(^)
#undef DEF_OP
