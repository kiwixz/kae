#pragma once

#include <new>
#include <type_traits>

#include "kae/c_ptr.h"

namespace kae::any_detail {

enum class ProxyOp {
    destroy,
    copy,
    move,
};

using Storage = CPtr < void, operator delete>;
using Proxy = void (*)(ProxyOp op, Storage* a, Storage* b);


struct Impl {
    Impl() = default;

    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Impl>, int> = 0>
    Impl(T&& value, Proxy proxy)
    {
        proxy_ = proxy;
        set_c_ptr(storage_, operator new(sizeof(T)));
        new (storage_.get()) std::decay_t<T>{std::forward<T>(value)};
    }

    ~Impl();
    Impl(const Impl& other);
    Impl& operator=(const Impl& other);
    Impl(Impl&& other) noexcept;
    Impl& operator=(Impl&& other) noexcept;

    explicit operator bool() const;

    template <typename T>
    T& as() const
    {
        return *std::launder(reinterpret_cast<T*>(storage_.get()));
    }

    void destroy();

    Storage storage_;
    Proxy proxy_;
};


template <typename T>
void proxy(ProxyOp op, Storage* a, Storage* b)
{
    switch (op) {
    case ProxyOp::destroy:
        std::launder(reinterpret_cast<T*>(a->get()))->~T();
        a->reset();
        break;
    case ProxyOp::copy:
        set_c_ptr(*b, operator new(sizeof(T)));
        new (b->get()) T{*std::launder(reinterpret_cast<const T*>(a->get()))};
        break;
    case ProxyOp::move:
        set_c_ptr(*b, operator new(sizeof(T)));
        new (b->get()) T{std::move(*std::launder(reinterpret_cast<T*>(a->get())))};
        break;
    }
}

template <typename T>
void proxy_unique(ProxyOp op, Storage* a, Storage* b)
{
    switch (op) {
    case ProxyOp::destroy:
        std::launder(reinterpret_cast<T*>(a->get()))->~T();
        a->reset();
        break;
    case ProxyOp::copy:
        std::terminate();
    case ProxyOp::move:
        set_c_ptr(*b, operator new(sizeof(T)));
        new (b->get()) T{std::move(*std::launder(reinterpret_cast<T*>(a->get())))};
        break;
    }
}

}  // namespace kae::any_detail
