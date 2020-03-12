#pragma once

#include <type_traits>

#include "kae/any_detail.h"

namespace kae {

struct Any {
    Any() = default;

    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Any>, int> = 0>
    Any(T&& value);

    explicit operator bool() const;

    template <typename T>
    T& as() const;

private:
    any_detail::Impl impl_;
};


struct UniqueAny {
    UniqueAny() = default;

    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, UniqueAny>, int> = 0>
    UniqueAny(T&& value);

    ~UniqueAny() = default;
    UniqueAny(const UniqueAny&) = delete;
    UniqueAny& operator=(const UniqueAny&) = delete;
    UniqueAny(UniqueAny&&) = default;
    UniqueAny& operator=(UniqueAny&&) = default;

    explicit operator bool() const;

    template <typename T>
    T& as() const;

private:
    any_detail::Impl impl_;
};


template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Any>, int>>
Any::Any(T&& value) :
    impl_{std::forward<T>(value), any_detail::proxy<std::decay_t<T>>}
{}

template <typename T>
T& Any::as() const
{
    return impl_.as<T>();
}


template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, UniqueAny>, int>>
UniqueAny::UniqueAny(T&& value) :
    impl_{std::forward<T>(value), any_detail::proxy_unique<std::decay_t<T>>}
{}

template <typename T>
T& UniqueAny::as() const
{
    return impl_.as<T>();
}

}  // namespace kae
